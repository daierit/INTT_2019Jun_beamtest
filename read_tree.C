using namespace std;
void read_tree(int RunNum){
  /**********Read root file*************************/
  char filename[200];
  char name[200];
  char path[200] = "/sphenix/data/data02/sphenix/t1439/2019/prdf/macro/";
  char map[200];
  sprintf(filename,"%sdata-%.8d.root",path,RunNum);
  TFile *f1= new TFile(filename);
  /*************************************************/

  TCanvas *c1 = new TCanvas("c1", "c1", 1900, 900);
  c1-> Divide(13,2);
TH1F 
  TH1F *chhit[26];
  for(int i=0; i<26; ++i){
    sprintf(name,"chhit%d",i);
    chhit[i] = new TH1F(name,name,128,0,128); 
  }
  for(int ifem=1; ifem<2; ++ifem){
    for(int ichip=0; ichip<26; ++ichip){
      c1-> cd(ichip+1);
      sprintf(name,"channel>>chhit%d",ichip+1);
      sprintf(map,"fem_id==%d&&chip==%d",ifem,ichip+26);
      online_debug_tree-> Draw(name,map);
      chhit[ichip]= (TH1F*)online_debug_tree
    }
  }
  sprintf(filename,"Run%d_channelhit.png",RunNum);
  c1-> Print(filename);
  //online_debug_tree->Draw("channel:chip","fem_id==0&&channel<127&&channel>0","colz");
} 

