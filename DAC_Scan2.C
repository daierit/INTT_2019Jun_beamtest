#include <TH1F.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TTree.h>
#include <iostream>
#include <string.h>
using namespace std;
void DAC_Scan2(const int ich, int RN_scan1, int RN_scan2, int RN_scan3, int RN_scan4, int RN_scan5, int RN_scan6, int RN_scan7, int RN_scan8, int RN_scan9, int RN_scan10, int RN_scan11, int RN_scan12, int RN_scan13){
  const int Scan_num=13;
  int Color[12]={600,603,618,616,632,634,402,416,418,420,435,433};
  char name[200];
  int RN_scan[Scan_num]={RN_scan1,RN_scan2,RN_scan3,RN_scan4,RN_scan5,RN_scan6,RN_scan7,RN_scan8,RN_scan9,RN_scan10,RN_scan11,RN_scan12,RN_scan13};
  //cout << RN_scan[0] << " " << RN_scan[1] << endl;
  char filename[200];
  char path[200] = "/sphenix/data/data02/sphenix/t1439/2019/prdf/macro/";
  TCanvas *c1 = new TCanvas("c1", "c1", 1200, 600);
  TH1F* rh1 = new TH1F("rh1","rh1",8,0,8);
  TH1F* DAC_Scan[Scan_num];
  c1-> cd();
  c1-> Divide(5,2);

  /*************Read data & nomalize********************/ 
  double nom;
  for(int ifem=1; ifem<2; ++ifem){
    for(int is=0; is<Scan_num; ++is){
      c1-> cd(is+1);
      //sprintf(path,"/sphenix/data/data02/sphenix/t1439/2019/prdf/macro");
      //sprintf(filename,"%s/data-%.8d.root",path,RN_scan[is]);
      sprintf(path,"/sphenix/u/kycheng/INTT_2019Jun_beamtest/adcdata");
      sprintf(filename,"%s/DAC-scan%d.root",path,RN_scan[is]);
      TFile *f1= new TFile(filename);
      sprintf(name,"DAC_Scan[%d]",is);
      DAC_Scan[is] = new TH1F(name,name,8,0,8);
      sprintf(name,"adc_value>>DAC_Scan[%d]",is);
      sprintf(path,"fem_id==%d&&chip==%d",ifem,ich);
      online_debug_tree-> Draw(name,path);
      DAC_Scan[is]-> SetDirectory(0);
      nom = DAC_Scan[is]-> GetEntries();
      nom -= DAC_Scan[is]-> GetBinContent(8);
      DAC_Scan[is]-> SetBinContent(8,0);
      if(is==0)
        nom -= DAC_Scan[is]-> GetBinContent(1);
      DAC_Scan[is]-> Scale(1.0/nom);
    }
    /*******************************************************/

    /******************Combine all scan*********************/
    TCanvas *c2 = new TCanvas("c2", "c2", 1200, 600);
    c2-> cd();
    TH1F*ADC_Full = new TH1F("ADC_Full","ADC_Full",63,8,259);
    ADC_Full-> GetXaxis()-> SetTitle("ADC");
    ADC_Full-> Draw();
    double ratio[2];
    double ratio_avg;
    double bin_N,max=0;
    for(int is=0; is<Scan_num; ++is){
      memset( (void *)ratio, 0, sizeof(double)*2 );
      bin_N=0,ratio_avg=0;
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
          ratio_avg = (ratio[0]+ratio[1])/bin_N;
          cout << ratio[0] << " " << ratio[1] << " " << ratio_avg << endl;
          DAC_Scan[is]-> Scale(ratio_avg);
          DAC_Scan[is]-> SetBins(8, 8+(is*8-3*is)*4,8+4*((is+1)*8-3*is));
          DAC_Scan[is]-> SetLineColor(Color[is]);
          DAC_Scan[is]-> SetFillStyle(4050);
          DAC_Scan[is]-> Draw("same");
          if(DAC_Scan[is]-> GetMaximum()>max)
            max = DAC_Scan[is]-> GetMaximum();
        } 
        else{ 
          if(is<4)
            break;
          else
            is=11;
        }
      }
      else if(is==0){
        DAC_Scan[is]-> SetBins(8, 8+(is*8-3*is)*4, 8+4*((is+1)*8-3*is));
        DAC_Scan[is]-> SetLineColor(Color[is]);
        DAC_Scan[is]-> SetFillStyle(4050);
        DAC_Scan[is]-> Draw("same");
      }
      else if(is==12){
        if(DAC_Scan[4]->GetBinContent(3)!=0 && DAC_Scan[is]->GetBinContent(1)!=0){
          ratio_avg += double(DAC_Scan[4]->GetBinContent(3))/double(DAC_Scan[is]->GetBinContent(1));
          ++bin_N;
        }
        if(DAC_Scan[4]->GetBinContent(4)!=0 && DAC_Scan[is]->GetBinContent(2)!=0){
          ratio_avg += double(DAC_Scan[4]->GetBinContent(4))/double(DAC_Scan[is]->GetBinContent(2));
          ++bin_N;
        }
        if(DAC_Scan[4]->GetBinContent(5)!=0 && DAC_Scan[is]->GetBinContent(3)!=0){
          ratio_avg += double(DAC_Scan[4]->GetBinContent(5))/double(DAC_Scan[is]->GetBinContent(3));
          ++bin_N;
        }
        if(DAC_Scan[4]->GetBinContent(6)!=0 && DAC_Scan[is]->GetBinContent(4)!=0){
          ratio_avg += double(DAC_Scan[4]->GetBinContent(6))/double(DAC_Scan[is]->GetBinContent(4));
          ++bin_N;
        }
        if(DAC_Scan[5]->GetBinContent(1)!=0 && DAC_Scan[is]->GetBinContent(4)!=0){
          ratio_avg += double(DAC_Scan[4]->GetBinContent(3))/double(DAC_Scan[is]->GetBinContent(4));
          ++bin_N;
        }
        if(DAC_Scan[4]->GetBinContent(7)!=0 && DAC_Scan[is]->GetBinContent(5)!=0){
          ratio_avg += double(DAC_Scan[4]->GetBinContent(7))/double(DAC_Scan[is]->GetBinContent(5));
          ++bin_N;
        }
        if(DAC_Scan[5]->GetBinContent(2)!=0 && DAC_Scan[is]->GetBinContent(5)!=0){
          ratio_avg += double(DAC_Scan[5]->GetBinContent(2))/double(DAC_Scan[is]->GetBinContent(5));
          ++bin_N;
        }
        if(DAC_Scan[5]->GetBinContent(3)!=0 && DAC_Scan[is]->GetBinContent(6)!=0){
          ratio_avg += double(DAC_Scan[5]->GetBinContent(3))/double(DAC_Scan[is]->GetBinContent(6));
          ++bin_N;
        }
        if(DAC_Scan[5]->GetBinContent(4)!=0 && DAC_Scan[is]->GetBinContent(7)!=0){
          ratio_avg += double(DAC_Scan[5]->GetBinContent(4))/double(DAC_Scan[is]->GetBinContent(7));
          ++bin_N;
        }
        cout << bin_N << endl;
        if(bin_N!=0){
          ratio_avg /= bin_N;
          DAC_Scan[is]-> Scale(ratio_avg);
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
    ADC_Full-> SetMaximum(max);
    sprintf(filename,"dacScan2_F%d_R%d_chip%d.pdf",ifem,RN_scan1,ich);
    c2-> Print(filename);
  }
  /***************************************************************************/
} 

