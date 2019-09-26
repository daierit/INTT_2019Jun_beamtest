#include <TH1F.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <iostream>
#include <string.h>
#include "langaus.C"
using namespace std;

//void DAC_Scan_Fit(const int ifem, const int ich){
int main(int argc, char *argv[]){
  int ifem = atoi(argv[1]);
  int ich = atoi(argv[2]);
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
    sprintf(path,"/home/kycheng/INTT_2019Jun_beamtest/adcdata");
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
      DAC_Scan[is]-> SetFillStyle(4050);
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
  TCanvas *c3 = new TCanvas("c3", "c3", 800, 600);
  TH1F *ADC_Full[26];
  TGraphErrors *FWidth = new TGraphErrors();
  TGraphErrors *FMIP = new TGraphErrors();
  double adc[63]; 
  int adc_c[63];
  c2-> Divide(3,2);
  //TF1* fun[26];
  int canvas=0;
  for(int ic=0; ic<26; ++ic){
    if( abs(ic-ich+27)<2 || abs(ic-ich+40)<2 ){
    c2-> cd(6-canvas);
    cout << "canvas:" << canvas << endl;
    memset( (void *)adc, 0, sizeof(double)*63 );
    memset( (void *)adc_c, 0, sizeof(int)*63 );
    for(int is=0; is<Scan_num; ++is){
      sprintf(path,"/home/kycheng/INTT_2019Jun_beamtest/adcdata");
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
      //cout << Form("ic%d is%d ",ic+27, is) << DAC_Scan[is]-> GetBinContent(1) << endl;
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
    double fr[2];
    double sv[4], pllo[4], plhi[4], fp[4], fpe[4];

    fr[0]=90.0; fr[1]=190.0;
    pllo[0]=0.5; pllo[1]=100.0; pllo[2]=1.0; pllo[3]=0.4;
    plhi[0]=5.0; plhi[1]=150.0; plhi[2]=1000000.0; plhi[3]=20.0;
    sv[0]=1.5; sv[1]=125.0; sv[2]=50000.0; sv[3]=10.0;
    double np[3], nps[3]={1,8,0.5}, npr[2]={0,50};

    double chisqr;
    int    ndf;
    TF1 *fitsnr = langaufit(ADC_Full[ic],fr,sv,pllo,plhi,fp,fpe,&chisqr,&ndf,nps,npr,np);

    double SNRPeak, SNRFWHM;
    langaupro(fp,SNRPeak,SNRFWHM);
    //fun[ic]= new TF1("fun","TMath::Landau(x,[0],[1],0)",0,190);
    //fun[ic]-> SetParameters(125,1.5);
    //ADC_Full[ic]-> Fit("fun");
    //cout << ADC_Full[ic]-> GetEntries() << endl;
    //nom = ADC_Full[ic]-> GetEntries();
    //ADC_Full[ic]-> Scale(1.0/nom);
    //cout << fp[0] << " " << fp[1] << " " << fp[2] << " " << fp[3] << endl;
    ADC_Full[ic]-> Draw();
    fitsnr-> Draw("lsame");
    FWidth-> SetPoint(canvas,canvas+1,fp[0]);
    FWidth-> SetPointError(canvas,0,fpe[0]);
    FMIP-> SetPoint(canvas,canvas+1,fp[1]);
    FMIP-> SetPointError(canvas,0,fpe[0]);
    //fun[ic]-> Draw("same");
    //cout << fun[ic]-> GetParameter() << endl;
    //ADC_Full[ic]-> SetMaximum(max);
    ++canvas;
    }
  }//ic
  c2-> Print(Form("dacScan_F%d_Fit.png",ifem));
  c3-> cd();
  FWidth-> SetMarkerStyle(20);
  FWidth-> SetMarkerSize(0.5);
  FWidth-> Draw("ap");
  FWidth-> Draw("p");
  c3-> Print(Form("dacScan_F%d_Fit_width.png",ifem));
  FMIP-> SetMarkerStyle(20);
  FMIP-> SetMarkerSize(0.5);
  FMIP-> Draw("ap");
  FMIP-> Draw("p");
  c3-> Print(Form("dacScan_F%d_Fit_MIP.png",ifem));
  /******************************************************************************/
  //}//ifem
} 

