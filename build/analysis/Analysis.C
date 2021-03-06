#include <iostream>
#include <fstream>
#include <string>
#include "TH2D.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TF1.h"

int Analysis(){

  std::cout << "========================== Analysis started! " << std::endl;
 
  double readvalue;
  std::vector<double> result[4];
  // result[0]:spread X / [1]:spread Y / [2]:Error X / [3]:Error Y
  
  std::string fileName[4];

#if 1
  /////////////////////////////////
  fileName[0]="../e1GeV/output.txt";
  fileName[1]="../e2GeV/output.txt";
  fileName[2]="../e3GeV/output.txt";
  fileName[3]="../e4GeV/output.txt";
  /////////////////////////////////
  std::cout << "No Rand Files!  "  << std::endl;
#elif 0
  /////////////////////////////////
  fileName[0]="../e1GeV/rand/output.txt";
  fileName[1]="../e2GeV/rand/output.txt";
  fileName[2]="../e3GeV/rand/output.txt";
  fileName[3]="../e4GeV/rand/output.txt";
  /////////////////////////////////
  std::cout << " Rand Filse!  "  << std::endl;
#endif
  
  std::string line;
  
  TGraph *grSpreadX = new TGraph;
  TGraph *grSpreadY = new TGraph;
  TGraph *grErrorX = new TGraph;
  TGraph *grErrorY = new TGraph;
  TH2D *frame[4];
  
  for(int nn=0;nn<4;nn++){
    if(nn<2) {
      //      frame[nn] = new TH2D(Form("frame%d",nn),"",1, 0, 5, 1, 0, 30);
      frame[nn] = new TH2D(Form("frame%d",nn),"",1, 0, 5, 1, 80, 120);
      frame[nn]->GetYaxis()->SetTitle("Sperad [#mum]");
    }else{
      frame[nn] = new TH2D(Form("frame%d",nn),"",1, 0, 5, 1, 0, 10);
      frame[nn]->GetYaxis()->SetTitle("Error [#mum]");
    }
    frame[nn]->GetXaxis()->SetTitle("Energy [GeV]");
    frame[nn]->SetStats(0);
  }
  
  for(int nn=0;nn<4;nn++){
    std::cout << "====================== E=" << nn+1 << "GeV" << std::endl;
    std::ifstream ifs(fileName[nn].c_str());
    if(!ifs){
      std::cout << "Can't open the file." << std::endl;
      return 0;
    }else{
      while(!ifs.eof()){
	//	std::cout << ifs.eof() << std::endl;
	ifs >> readvalue;
	result[nn].push_back(readvalue);
	std::cout << readvalue << std::endl;
      }
    }
  }
  
  
  
  for(int nn=0;nn<4;nn++){
    grSpreadX->SetPoint(nn,nn+1,result[nn][0]);
    grSpreadY->SetPoint(nn,nn+1,result[nn][1]);
    grErrorX->SetPoint(nn,nn+1,result[nn][2]);
    grErrorY->SetPoint(nn,nn+1,result[nn][3]);    
  }
  /*  
  TCanvas *c = new TCanvas("c","Energy vs Spread",800,1600);
  c->Divide(1,2);
  c->cd(1);
  frame[0]->SetTitle("Energy vs X Spread");
  gPad->SetGrid(1,1);
  frame[0]->Draw();
  grSpreadX->SetLineColor(2);
  grSpreadX->SetMarkerStyle(22);
  grSpreadX->SetMarkerColor(2);
  grSpreadX->SetMarkerSize(2);  
  grSpreadX->Draw("P same");
  c->cd(2);
  frame[1]->SetTitle("Energy vs Y Spread");
  gPad->SetGrid(1,1);
  frame[1]->Draw();
  grSpreadY->SetLineColor(2);
  grSpreadY->SetMarkerStyle(22);
  grSpreadY->SetMarkerColor(4);
  grSpreadY->SetMarkerSize(2);  
  grSpreadY->Draw("P same");
  c->Print("Spread1um1000px.png");
  */
  
  TCanvas *c1 = new TCanvas("c1","Energy vs Error",800,1600);
  c1->Divide(1,2);
  c1->cd(1);
  frame[2]->SetTitle("Energy vs X Error");
  gPad->SetGrid(1,1);
  frame[2]->Draw();
  grErrorX->SetLineColor(2);
  grErrorX->SetMarkerStyle(22);
  grErrorX->SetMarkerColor(2);
  grErrorX->SetMarkerSize(2);  
  grErrorX->Draw("P same");
  c1->cd(2);
  frame[3]->SetTitle("Energy vs Y Error");
  gPad->SetGrid(1,1);
  frame[3]->Draw();
  grErrorY->SetLineColor(2);
  grErrorY->SetMarkerStyle(22);
  grErrorY->SetMarkerColor(4);
  grErrorY->SetMarkerSize(2);  
  grErrorY->Draw("P same");
  c1->Print("Error.png");

  TFile *fout = new TFile("px8.root","RECREATE");
  grErrorX->Write();
  grErrorY->Write();
  
  return 0;
  
}
