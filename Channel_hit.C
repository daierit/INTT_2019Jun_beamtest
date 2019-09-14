#include <TH1F.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TTree.h>
#include <iostream>
#include <string.h>
#include <fstream>
using namespace std;

void Channel_hit(){
  int Color[12]={600,603,618,616,632,634,402,416,418,420,435,433};
  char name[200];
  char filename[200];
  char path[200] = "/sphenix/data/data02/sphenix/t1439/2019/prdf/macro/";
  TCanvas *c1 = new TCanvas("c1", "c1", 1800, 1100);
  TH1F* Chit[104];
  c1-> Divide(13,8);
  //TCanvas *c2 = new TCanvas("c2","c2",1200,900);
  //c2-> Divide(2,2);
  //TH1F* Chipmax[4];
  /*************Read RunNumber******************/
  fstream r1;
  vector<int> Run;
  int buff;
  r1.open("HitRun.txt",ios::in);
  //if(r1){
    //while(r1>>buff)
      //Run.push_back(buff);
 // }
  Run.push_back(617);
  /*********************************************/
  /*************Read data********************************/ 
  for(int run=0; run<Run.size(); ++run){
    sprintf(path,"/sphenix/data/data02/sphenix/t1439/2019/prdf/macro");
    sprintf(filename,"%s/data-%.8d.root",path,Run[run]);
    TFile *f1= TFile::Open(filename);
    int flip[128];
    for(int ifem=0;ifem<4;++ifem){
      int max=0;
      //Chipmax[ifem] = new TH1F(Form("%dmax%d",Run[run],ifem),Form("%dmax%d",Run[run],ifem),26,0,26);
      for(int ich=0; ich<26; ++ich){
        c1-> cd(26*ifem+26-ich);
        sprintf(name,"channel>>rh1_%d_%d_%d",Run[run],ifem,ich);
        sprintf(path,"fem_id==%d&&chip==%d",ifem,ich+27);
        TTree* online_debug_tree = (TTree*)f1-> Get("online_debug_tree");
        online_debug_tree-> Draw(name,path);
        //sprintf(name,"Chit[%d]",ich);
        Chit[ich+ifem*26] = (TH1F*)(f1-> Get(Form("rh1_%d_%d_%d",Run[run],ifem,ich))-> Clone());
        //Chit[ich]-> SetName(name);
        Chit[ich+ifem*26]-> SetDirectory(0);
        if(ich>12){
          for(int ic=0;ic<128;++ic)
            flip[127-ic]= Chit[ich+ifem*26]-> GetBinContent(ic+1);
          for(int ic=0;ic<128;++ic)
            Chit[ich+ifem*26]-> SetBinContent(ic+1,flip[ic]);
        }
        //if(Chit[ich]-> GetMaximum()>max && Chit[ich]-> GetMaximum()<2000)
          //max=Chit[ich]-> GetMaximum();
        if(ich==6){
          max=Chit[ich+ifem*26]-> GetMaximum();
          cout << max << endl;
        }
        //Chipmax[ifem]-> SetBinContent(ich,Chit[ich]-> GetMaximum());
        //cout << ifem << " " << ich << " " << Chit[ich]-> GetMaximum() << endl;
      }
      for(int ich=0; ich<26; ++ich){
        c1-> cd(26*ifem+26-ich);
        Chit[ich+ifem*26]-> SetMaximum(max);
        Chit[ich+ifem*26]-> Draw("hbar");
      }
      //c2-> cd(ifem+1);
      //Chipmax[ifem]-> Draw();
    }
    sprintf(name,"/sphenix/u/kycheng/INTT_2019Jun_beamtest/ChannelHit/Run%.3d.png",Run[run]);
    c1-> Print(name);
    f1-> Close();
  }
  /*******************************************************/
} 

