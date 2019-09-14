#include <TH1F.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TTree.h>
#include <iostream>
#include <string.h>
using namespace std;

void DAC_Scan_Full(const int ifem, const int ich){
  const int Scan_num=13;
  int Color[12]={600,603,618,616,632,634,402,416,418,420,435,433};
  char name[200];
  //cout << RN_scan[0] << " " << RN_scan[1] << endl;
  char filename[200];
  char path[200] = "/sphenix/data/data02/sphenix/t1439/2019/prdf/macro/";
  TCanvas *c1 = new TCanvas("c1", "c1", 1200, 600);
  TH1F* DAC_Scan[Scan_num];
  c1-> cd();

  /*************Read data & nomalize********************/ 
  double nom;
  //for(int ifem=1; ifem<2; ++ifem){
  for(int is=0; is<Scan_num; ++is){
    //sprintf(path,"/sphenix/data/data02/sphenix/t1439/2019/prdf/macro");
    //sprintf(filename,"%s/data-%.8d.root",path,RN_scan[is]);
    sprintf(path,"/sphenix/u/kycheng/INTT_2019Jun_beamtest/adcdata");
    sprintf(filename,"%s/DAC-scan%d.root",path,is+1);
    TFile *f1= TFile::Open(filename);
    sprintf(name,"adc_value>>rh1_%d",is);
    sprintf(path,"fem_id==%d&&chip==%d",ifem,ich);
    TTree* online_debug_tree = (TTree*)f1-> Get("online_debug_tree");
    online_debug_tree-> Draw(name,path);
    sprintf(name,"DAC_Scan[%d]",is);
    DAC_Scan[is] = (TH1F*)(f1-> Get(Form("rh1_%d",is))-> Clone());
    DAC_Scan[is]-> SetName(name);
    DAC_Scan[is]-> SetDirectory(0);
    nom = DAC_Scan[is]-> GetEntries();
    nom -= DAC_Scan[is]-> GetBinContent(8);
    //cout << DAC_Scan[is]-> GetBinContent(8) << endl;
    if(is==0)
      nom -= DAC_Scan[is]-> GetBinContent(1);
    DAC_Scan[is]-> Scale(1.0/nom);
    DAC_Scan[is]-> SetBinContent(8,0);
    if(is==0)
      DAC_Scan[is]-> SetBinContent(1,0);
    f1-> Close();
    cout << is << " " << DAC_Scan[is]-> GetBinContent(8) << endl;
  }
  /*******************************************************/

  /******************Find ratio of DAC Scan*********************/
  double ratio[2];
  double ratio_avg[13];
  double bin_N,max=0;
  memset( (void *)ratio_avg, 0, sizeof(double)*13 );
  TH1F* ADC_Canvas = new TH1F("ADC_Canvas","ADC_Canvas",63,8,260);
  ADC_Canvas-> Draw();
  for(int is=0; is<Scan_num; ++is){
    memset( (void *)ratio, 0, sizeof(double)*2 );
    bin_N=0;
    /*****find ration between two scan*****/
    if(is>0 && is<12){
      if(DAC_Scan[is-1]->GetBinContent(6)!=0 && DAC_Scan[is]->GetBinContent(1)!=0){
        ratio[0] = double(DAC_Scan[is-1]->GetBinContent(6))/double(DAC_Scan[is]->GetBinContent(1));
        ++bin_N;
        //cout << DAC_Scan[0]->GetBinContent(5) << "/" << DAC_Scan[1]->GetBinContent(1) << endl;
      }
      if(DAC_Scan[is-1]->GetBinContent(7)!=0 && DAC_Scan[is]->GetBinContent(2)!=0){
        ratio[1] = double(DAC_Scan[is-1]->GetBinContent(7))/double(DAC_Scan[is]->GetBinContent(2));
        ++bin_N;
        //cout << DAC_Scan[0]->GetBinContent(6) << "/" << DAC_Scan[1]->GetBinContent(2) << endl;
      }
      if(bin_N!=0){
        ratio_avg[is] = (ratio[0]+ratio[1])/bin_N;
        cout << ratio[0] << " " << ratio[1] << " " << ratio_avg[is] << endl;
        DAC_Scan[is]-> Scale(ratio_avg[is]);
        DAC_Scan[is]-> SetBins(8, 8+(is*8-3*is)*4,8+4*((is+1)*8-3*is));
        DAC_Scan[is]-> SetLineColor(Color[is]);
        DAC_Scan[is]-> SetFillStyle(4050);
        DAC_Scan[is]-> Draw("same");
        if(DAC_Scan[is]-> GetMaximum()>max)
          max = DAC_Scan[is]-> GetMaximum();
      } 
      else
        break;
    }
    else if(is==0){
      DAC_Scan[is]-> SetBins(8, 8+(is*8-3*is)*4, 8+4*((is+1)*8-3*is));
      DAC_Scan[is]-> SetLineColor(Color[is]);
      DAC_Scan[is]-> SetFillStyle(4050);
      DAC_Scan[is]-> Draw("same");
    }
    else if(is==12){
      if(DAC_Scan[4]->GetBinContent(3)!=0 && DAC_Scan[is]->GetBinContent(1)!=0){
        ratio_avg[is] += double(DAC_Scan[4]->GetBinContent(3))/double(DAC_Scan[is]->GetBinContent(1));
        ++bin_N;
      }
      if(DAC_Scan[4]->GetBinContent(4)!=0 && DAC_Scan[is]->GetBinContent(2)!=0){
        ratio_avg[is] += double(DAC_Scan[4]->GetBinContent(4))/double(DAC_Scan[is]->GetBinContent(2));
        ++bin_N;
      }
      if(DAC_Scan[4]->GetBinContent(5)!=0 && DAC_Scan[is]->GetBinContent(3)!=0){
        ratio_avg[is] += double(DAC_Scan[4]->GetBinContent(5))/double(DAC_Scan[is]->GetBinContent(3));
        ++bin_N;
      }
      if(DAC_Scan[4]->GetBinContent(6)!=0 && DAC_Scan[is]->GetBinContent(4)!=0){
        ratio_avg[is] += double(DAC_Scan[4]->GetBinContent(6))/double(DAC_Scan[is]->GetBinContent(4));
        ++bin_N;
      }
      if(DAC_Scan[5]->GetBinContent(1)!=0 && DAC_Scan[is]->GetBinContent(4)!=0){
        ratio_avg[is] += double(DAC_Scan[4]->GetBinContent(3))/double(DAC_Scan[is]->GetBinContent(4));
        ++bin_N;
      }
      if(DAC_Scan[4]->GetBinContent(7)!=0 && DAC_Scan[is]->GetBinContent(5)!=0){
        ratio_avg[is] += double(DAC_Scan[4]->GetBinContent(7))/double(DAC_Scan[is]->GetBinContent(5));
        ++bin_N;
      }
      if(DAC_Scan[5]->GetBinContent(2)!=0 && DAC_Scan[is]->GetBinContent(5)!=0){
        ratio_avg[is] += double(DAC_Scan[5]->GetBinContent(2))/double(DAC_Scan[is]->GetBinContent(5));
        ++bin_N;
      }
      if(DAC_Scan[5]->GetBinContent(3)!=0 && DAC_Scan[is]->GetBinContent(6)!=0){
        ratio_avg[is] += double(DAC_Scan[5]->GetBinContent(3))/double(DAC_Scan[is]->GetBinContent(6));
        ++bin_N;
      }
      if(DAC_Scan[5]->GetBinContent(4)!=0 && DAC_Scan[is]->GetBinContent(7)!=0){
        ratio_avg[is] += double(DAC_Scan[5]->GetBinContent(4))/double(DAC_Scan[is]->GetBinContent(7));
        ++bin_N;
      }
      cout << bin_N << endl;
      if(bin_N!=0){
        ratio_avg[is] /= bin_N;
        DAC_Scan[is]-> Scale(ratio_avg[is]);
        DAC_Scan[is]-> SetBins(8,96,128);
        DAC_Scan[is]-> SetLineColor(1);
        DAC_Scan[is]-> SetFillStyle(4050);
        DAC_Scan[is]-> Draw("same");
        if(DAC_Scan[is]-> GetMaximum()>max)
          max = DAC_Scan[is]-> GetMaximum();
      }
    }
    //cout << "1: " << DAC_Scan[0]->GetBinContent(6) << " " << DAC_Scan[1]->GetBinContent(1) << endl;
    //cout << "2: " << DAC_Scan[0]->GetBinContent(7) << " " << DAC_Scan[1]->GetBinContent(2) << endl;
    //ADC_Full-> SetMaximum(0.1);
  }

  for(int is=1;is<Scan_num;++is){
    cout << ratio_avg[is] << endl;
  }
  cout << max << endl;
  ADC_Canvas-> SetMaximum(max);
  sprintf(filename,"dacScan2_F%d_chip%d.png",ifem,ich);
  c1-> Print(filename);
  /***************************************************************************/
  cout << max << endl;
  /******************Combine DAC Scan for all chip*******************************/
  TCanvas *c2 = new TCanvas("c2", "c2", 1920, 1080); 
  TH1F*ADC_Full[26];
  double adc[63]; 
  int adc_c[63];
  c2-> cd();
  c2-> Divide(13,2);
  for(int ic=0; ic<26; ++ic){
    c2-> cd(26-ic);
    memset( (void *)adc, 0, sizeof(double)*63 );
    memset( (void *)adc_c, 0, sizeof(int)*63 );
    for(int is=0; is<Scan_num; ++is){
      sprintf(path,"/sphenix/u/kycheng/INTT_2019Jun_beamtest/adcdata");
      sprintf(filename,"%s/DAC-scan%d.root",path,is+1);
      TFile *f1= TFile::Open(filename);
      sprintf(name,"adc_value>>rh2_%d",is);
      sprintf(path,"fem_id==%d&&chip==%d",ifem,ic+27);
      TTree* online_debug_tree = (TTree*)f1-> Get("online_debug_tree");
      online_debug_tree-> Draw(name,path);
      sprintf(name,"DAC_Scan%d[%d]",ic,is);
      DAC_Scan[is] = (TH1F*)(f1-> Get(Form("rh2_%d",is))-> Clone());
      DAC_Scan[is]-> SetName(name);
      DAC_Scan[is]-> SetDirectory(0);
      //for(int bin=0; bin<7; ++bin)
      //cout << Form("ic%d is%d ",ic+27, is) << DAC_Scan[is]-> GetBinContent(bin+1) << endl;
      nom = DAC_Scan[is]-> GetEntries();
      nom -= DAC_Scan[is]-> GetBinContent(8);
      if(is==0)
        nom -= DAC_Scan[is]-> GetBinContent(1);
      DAC_Scan[is]-> Scale(1.0/nom);
      if(is>0 && ratio_avg[is]>0)
        DAC_Scan[is]-> Scale(ratio_avg[is]);
      DAC_Scan[is]-> SetBinContent(8,0);
      if(is==0)
        DAC_Scan[is]-> SetBinContent(1,0);
      for(int bin=0; bin<7; ++bin){
        if(is<12 && ratio_avg[is]>0){
          adc[is*5+bin]+= DAC_Scan[is]-> GetBinContent(bin+1);
          ++adc_c[is*5+bin];
          //cout << DAC_Scan[is]-> GetBinContent(bin+1) << endl;;
        }
        else if(is==12 && ratio_avg[is]>0){
          adc[22+bin]+= DAC_Scan[is]-> GetBinContent(bin+1);
          ++adc_c[22+bin];
        }
        else if(is==0){
          adc[is*5+bin]+= DAC_Scan[is]-> GetBinContent(bin+1);
          ++adc_c[is*5+bin];
        }
      }//bin
      f1-> Close();
    }//is
    ADC_Full[ic] = new TH1F(Form("ADC_Full[%d]",ic),Form("ADC_Full[%d]",ic+27),63,8,260);
    nom=0;
    for(int bin=0; bin<63; ++bin){
      if(adc[bin]/adc_c[bin] > 0){
        ADC_Full[ic]-> SetBinContent(bin+1,adc[bin]/adc_c[bin]);
        nom += adc[bin]/adc_c[bin];
        //cout << Form("ic%d",ic) << " " << bin << " " << adc[bin]/adc_c[bin] << endl;
      }
    }
    //cout << ADC_Full[ic]-> GetEntries() << endl;
    //nom = ADC_Full[ic]-> GetEntries();
    ADC_Full[ic]-> Scale(1.0/nom);
    ADC_Full[ic]-> Draw();
    //ADC_Full[ic]-> SetMaximum(max);
  }//ic
  c2-> Print(Form("dacScan_F%d.pdf",ifem));
  /******************************************************************************/
  //}//ifem
} 

