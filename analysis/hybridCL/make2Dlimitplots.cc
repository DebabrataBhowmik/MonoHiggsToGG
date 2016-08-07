#include <TFile.h>
#include <TLine.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TMath.h>
#include "../macro/mkPlotsLivia/CMS_lumi.C"
#include <iostream>
#include <vector>

void makePlots(TString, TString);
void getLimits(TFile*, Double_t &, Double_t); 
void getXsec(TFile*, UInt_t, UInt_t, Double_t &); 

void make2Dlimitplots(){
 
  cout << "Making 2D limit plots" << endl;

  TString inDir = "~soffi/public/4Margaret/2Dinputs/";
  //TString inDir = "~/private/HiggsCombine/CMSSW_7_1_5/src/diphotons/Analysis/macros/";
  TString outDir = "~/www/Plots/25ns_Limits_76X_2DResults/";
  // SPECIFY LUMI in mkPlotsLivia/CMS_lumi.C

  makePlots(inDir, outDir);

}


void makePlots(TString inDir, TString outDir){

  // mZp masses 
  Double_t mass[8] = {600,800,1000,1200,1400,1700,2000,2500};
  Int_t nMasses = 8;

  // pick up the higgsCombine files for each A0 mass
  std::vector<TFile* > higgsCombineFiles_MA0300;
  std::vector<TFile* > higgsCombineFiles_MA0400;
  std::vector<TFile* > higgsCombineFiles_MA0500;
  std::vector<TFile* > higgsCombineFiles_MA0600;
  std::vector<TFile* > higgsCombineFiles_MA0700;
  std::vector<TFile* > higgsCombineFiles_MA0800;

  higgsCombineFiles_MA0300.resize(nMasses);
  higgsCombineFiles_MA0400.resize(nMasses);
  higgsCombineFiles_MA0500.resize(nMasses);
  higgsCombineFiles_MA0600.resize(nMasses);
  higgsCombineFiles_MA0700.resize(nMasses);
  higgsCombineFiles_MA0800.resize(nMasses);

  for (UInt_t n=0; n<nMasses; n++){
    higgsCombineFiles_MA0300[n] = new TFile(Form("%sMA300/higgsCombineTest.HybridNew.mH%i.root",inDir.Data(),(Int_t)mass[n]));
    higgsCombineFiles_MA0400[n] = new TFile(Form("%sMA400/higgsCombineTest.HybridNew.mH%i.root",inDir.Data(),(Int_t)mass[n]));
    higgsCombineFiles_MA0500[n] = new TFile(Form("%sMA500/higgsCombineTest.HybridNew.mH%i.root",inDir.Data(),(Int_t)mass[n]));
    higgsCombineFiles_MA0600[n] = new TFile(Form("%sMA600/higgsCombineTest.HybridNew.mH%i.root",inDir.Data(),(Int_t)mass[n]));
    higgsCombineFiles_MA0700[n] = new TFile(Form("%sMA700/higgsCombineTest.HybridNew.mH%i.root",inDir.Data(),(Int_t)mass[n]));
    higgsCombineFiles_MA0800[n] = new TFile(Form("%sMA800/higgsCombineTest.HybridNew.mH%i.root",inDir.Data(),(Int_t)mass[n]));
  }

 // pick up theory xsec
 TFile* theory_gz08 = new TFile("ScanPlot_gz08.root");

 // setup 1D plots - expected
 TGraph* limit300;
 TGraph* limit400;
 TGraph* limit500;
 TGraph* limit600;
 TGraph* limit700;
 TGraph* limit800;

 // setup 1D plots - observed
 TGraph* limit300_obs;
 TGraph* limit400_obs;
 TGraph* limit500_obs;
 TGraph* limit600_obs;
 TGraph* limit700_obs;
 TGraph* limit800_obs;

 // setup output plot
 TH2D * limits = new TH2D("limits","limits",8,0,8,6,0,6);
 limits->GetXaxis()->SetTitle("m_{Z'} [GeV]");
 limits->GetYaxis()->SetTitle("m_{A0} [GeV]");
 limits->SetTitle("");
 limits->SetMaximum(3000);
 limits->SetMarkerSize(2);

 // set the lables for the Xaxis (mZp)
 limits->GetXaxis()->SetBinLabel(1,"600");
 limits->GetXaxis()->SetBinLabel(2,"800");
 limits->GetXaxis()->SetBinLabel(3,"1000");
 limits->GetXaxis()->SetBinLabel(4,"1200");
 limits->GetXaxis()->SetBinLabel(5,"1400");
 limits->GetXaxis()->SetBinLabel(6,"1700");
 limits->GetXaxis()->SetBinLabel(7,"2000");
 limits->GetXaxis()->SetBinLabel(8,"2500");

 // set the lables for the Yaxis (mA0)
 limits->GetYaxis()->SetBinLabel(1,"300");
 limits->GetYaxis()->SetBinLabel(2,"400");
 limits->GetYaxis()->SetBinLabel(3,"500");
 limits->GetYaxis()->SetBinLabel(4,"600");
 limits->GetYaxis()->SetBinLabel(5,"700");
 limits->GetYaxis()->SetBinLabel(6,"800");

 // setup output observed plot
 TH2D * obslimits = (TH2D*) limits->Clone();

 // setup canvas
 TCanvas * c = new TCanvas("c","");
 c->cd();
 gStyle->SetOptStat(0);
 gStyle->SetPaintTextFormat("2.1f");

 Double_t limitval300[nMasses];
 Double_t limitval400[nMasses];
 Double_t limitval500[nMasses];
 Double_t limitval600[nMasses];
 Double_t limitval700[nMasses];
 Double_t limitval800[nMasses];

 Double_t limitval300_obs[nMasses];
 Double_t limitval400_obs[nMasses];
 Double_t limitval500_obs[nMasses];
 Double_t limitval600_obs[nMasses];
 Double_t limitval700_obs[nMasses];
 Double_t limitval800_obs[nMasses];

 Double_t xsecA0300[nMasses];
 Double_t xsecA0400[nMasses];
 Double_t xsecA0500[nMasses];
 Double_t xsecA0600[nMasses];
 Double_t xsecA0700[nMasses];
 Double_t xsecA0800[nMasses];

 Double_t explimit300[nMasses];
 Double_t explimit400[nMasses];
 Double_t explimit500[nMasses];
 Double_t explimit600[nMasses];
 Double_t explimit700[nMasses];
 Double_t explimit800[nMasses];
 
 Double_t obslimit300[nMasses];
 Double_t obslimit400[nMasses];
 Double_t obslimit500[nMasses];
 Double_t obslimit600[nMasses];
 Double_t obslimit700[nMasses];
 Double_t obslimit800[nMasses];

 for (Int_t n=0; n<nMasses; n++){
   getLimits(higgsCombineFiles_MA0300[n],limitval300[n],0.5); 
   getLimits(higgsCombineFiles_MA0400[n],limitval400[n],0.5); 
   getLimits(higgsCombineFiles_MA0500[n],limitval500[n],0.5); 
   getLimits(higgsCombineFiles_MA0600[n],limitval600[n],0.5); 
   getLimits(higgsCombineFiles_MA0700[n],limitval700[n],0.5); 
   getLimits(higgsCombineFiles_MA0800[n],limitval800[n],0.5); 
   //std::cout<<n<<" "<<limitval300[n]<< " "<<limitval400[n]<<limitval500[n]<< " "<<limitval600[n] <<limitval700[n]<< " "<<limitval800[n]<<std::endl;

   getLimits(higgsCombineFiles_MA0300[n],limitval300_obs[n],-1); 
   getLimits(higgsCombineFiles_MA0400[n],limitval400_obs[n],-1); 
   getLimits(higgsCombineFiles_MA0500[n],limitval500_obs[n],-1); 
   getLimits(higgsCombineFiles_MA0600[n],limitval600_obs[n],-1); 
   getLimits(higgsCombineFiles_MA0700[n],limitval700_obs[n],-1); 
   getLimits(higgsCombineFiles_MA0800[n],limitval800_obs[n],-1); 

   getXsec(theory_gz08,300,(Int_t)mass[n],xsecA0300[n]);
   getXsec(theory_gz08,400,(Int_t)mass[n],xsecA0400[n]);
   getXsec(theory_gz08,500,(Int_t)mass[n],xsecA0500[n]);
   getXsec(theory_gz08,600,(Int_t)mass[n],xsecA0600[n]);
   getXsec(theory_gz08,700,(Int_t)mass[n],xsecA0700[n]);
   getXsec(theory_gz08,800,(Int_t)mass[n],xsecA0800[n]);

   explimit300[n] = limitval300[n]/xsecA0300[n];
   explimit400[n] = limitval400[n]/xsecA0400[n];
   explimit500[n] = limitval500[n]/xsecA0500[n];
   explimit600[n] = limitval600[n]/xsecA0600[n];
   explimit700[n] = limitval700[n]/xsecA0700[n];
   explimit800[n] = limitval800[n]/xsecA0800[n];

   obslimit300[n] = limitval300_obs[n]/xsecA0300[n];
   obslimit400[n] = limitval400_obs[n]/xsecA0400[n];
   obslimit500[n] = limitval500_obs[n]/xsecA0500[n];
   obslimit600[n] = limitval600_obs[n]/xsecA0600[n];
   obslimit700[n] = limitval700_obs[n]/xsecA0700[n];
   obslimit800[n] = limitval800_obs[n]/xsecA0800[n];

   // fill limit plot
   limits->Fill(((Double_t)n+0.5),0.5,limitval300[n]/xsecA0300[n]);
   limits->Fill(((Double_t)n+0.5),1.5,limitval400[n]/xsecA0400[n]);
   limits->Fill(((Double_t)n+0.5),2.5,limitval500[n]/xsecA0500[n]);
   limits->Fill(((Double_t)n+0.5),3.5,limitval600[n]/xsecA0600[n]);
   limits->Fill(((Double_t)n+0.5),4.5,limitval700[n]/xsecA0700[n]);
   limits->Fill(((Double_t)n+0.5),5.5,limitval800[n]/xsecA0800[n]);

   obslimits->Fill(((Double_t)n+0.5),0.5,limitval300_obs[n]/xsecA0300[n]);
   obslimits->Fill(((Double_t)n+0.5),1.5,limitval400_obs[n]/xsecA0400[n]);
   obslimits->Fill(((Double_t)n+0.5),2.5,limitval500_obs[n]/xsecA0500[n]);
   obslimits->Fill(((Double_t)n+0.5),3.5,limitval600_obs[n]/xsecA0600[n]);
   obslimits->Fill(((Double_t)n+0.5),4.5,limitval700_obs[n]/xsecA0700[n]);
   obslimits->Fill(((Double_t)n+0.5),5.5,limitval800_obs[n]/xsecA0800[n]);
 }

 // only pick up the limits that are non-zero
 Double_t mass_400[7] = {600,800,1000,1200,1400,1700,2000};
 Double_t mass_500[7] = {800,1000,1200,1400,1700,2000,2500};
 Double_t mass_600[5] = {1000,1400,1700,2000,2500};
 Double_t mass_700[6] = {1000,1200,1400,1700,2000,2500};
 Double_t mass_800[6] = {1000,1200,1400,1700,2000,2500};
 Double_t limitval_exp_400[8] = {explimit400[0],explimit400[1],explimit400[2],explimit400[3],explimit400[4],explimit400[5],explimit400[6]};
 Double_t limitval_obs_400[8] = {obslimit400[0],obslimit400[1],obslimit400[2],obslimit400[3],obslimit400[4],obslimit400[5],obslimit400[6]};
 
 Double_t limitval_exp_500[8] = {explimit500[1],explimit500[2],explimit500[3],explimit500[4],explimit500[5],explimit500[6],explimit500[7]};
 Double_t limitval_obs_500[8] = {obslimit500[1],obslimit500[2],obslimit500[3],obslimit500[4],obslimit500[5],obslimit500[6],obslimit500[7]};

 Double_t limitval_exp_600[8] = {explimit600[2],explimit600[4],explimit600[5],explimit600[6],explimit600[7]};
 Double_t limitval_obs_600[8] = {obslimit600[2],obslimit600[4],obslimit600[5],obslimit600[6],obslimit600[7]};

 Double_t limitval_exp_700[8] = {explimit700[2],explimit700[3],explimit700[4],explimit700[5],explimit700[6],explimit700[7]};
 Double_t limitval_obs_700[8] = {obslimit700[2],obslimit700[3],obslimit700[4],obslimit700[5],obslimit700[6],obslimit700[7]};

 Double_t limitval_exp_800[8] = {explimit800[2],explimit800[3],explimit800[4],explimit800[5],explimit800[6],explimit800[7]};
 Double_t limitval_obs_800[8] = {obslimit800[2],obslimit800[3],obslimit800[4],obslimit800[5],obslimit800[6],obslimit800[7]};

 limit300 = new TGraph(nMasses,mass,explimit300);
 limit400 = new TGraph(7,mass_400,limitval_exp_400);
 limit500 = new TGraph(7,mass_500,limitval_exp_500);
 limit600 = new TGraph(5,mass_600,limitval_exp_600);
 limit700 = new TGraph(6,mass_700,limitval_exp_700);
 limit800 = new TGraph(6,mass_800,limitval_exp_800);
 
 limit300_obs = new TGraph(nMasses,mass,obslimit300);
 limit400_obs = new TGraph(7,mass_400,limitval_obs_400);
 limit500_obs = new TGraph(7,mass_500,limitval_obs_500);
 limit600_obs = new TGraph(5,mass_600,limitval_obs_600);
 limit700_obs = new TGraph(6,mass_700,limitval_obs_700);
 limit800_obs = new TGraph(6,mass_800,limitval_obs_800);


 //styling
 limit300->GetXaxis()->SetTitle("m_{Z'} [GeV]");
 limit300->GetYaxis()->SetTitle("UL");
 limit300->SetTitle("");
 limit300->SetMaximum(3000);
 limit300->SetMinimum(0.9);
 limit300->SetLineWidth(2);
 limit400->SetLineWidth(2);
 limit500->SetLineWidth(2);
 limit600->SetLineWidth(2);
 limit700->SetLineWidth(2);
 limit800->SetLineWidth(2);
 limit300->SetMarkerStyle(8);
 limit400->SetMarkerStyle(8);
 limit500->SetMarkerStyle(8);
 limit600->SetMarkerStyle(8);
 limit700->SetMarkerStyle(8);
 limit800->SetMarkerStyle(8);
 // set up colors to match Hbb
 limit300->SetLineColor(kBlack);
 limit400->SetLineColor(kCyan);
 limit500->SetLineColor(kGreen);
 limit600->SetLineColor(kBlue);
 limit700->SetLineColor(kYellow);
 limit800->SetLineColor(kMagenta);
 limit300->SetMarkerColor(kBlack);
 limit400->SetMarkerColor(kCyan);
 limit500->SetMarkerColor(kGreen);
 limit600->SetMarkerColor(kBlue);
 limit700->SetMarkerColor(kYellow);
 limit800->SetMarkerColor(kMagenta);


 //styling
 limit300_obs->GetXaxis()->SetTitle("m_{Z'} [GeV]");
 limit300_obs->GetYaxis()->SetTitle("UL [fb]");
 limit300_obs->SetTitle("");
 limit300_obs->SetMaximum(3000);
 limit300_obs->SetMinimum(0.9);
 limit300_obs->SetLineWidth(2);
 limit400_obs->SetLineWidth(2);
 limit500_obs->SetLineWidth(2);
 limit600_obs->SetLineWidth(2);
 limit700_obs->SetLineWidth(2);
 limit800_obs->SetLineWidth(2);
 limit300_obs->SetMarkerStyle(8);
 limit400_obs->SetMarkerStyle(8);
 limit500_obs->SetMarkerStyle(8);
 limit600_obs->SetMarkerStyle(8);
 limit700_obs->SetMarkerStyle(8);
 limit800_obs->SetMarkerStyle(8);
 // set up colors to match Hbb
 limit300_obs->SetMarkerColor(kBlack);
 limit400_obs->SetMarkerColor(kCyan);
 limit500_obs->SetMarkerColor(kGreen);
 limit600_obs->SetMarkerColor(kBlue);
 limit700_obs->SetMarkerColor(kYellow);
 limit800_obs->SetMarkerColor(kMagenta);
 limit300_obs->SetLineColor(kBlack);
 limit400_obs->SetLineColor(kCyan);
 limit500_obs->SetLineColor(kGreen);
 limit600_obs->SetLineColor(kBlue);
 limit700_obs->SetLineColor(kYellow);
 limit800_obs->SetLineColor(kMagenta);
 
 // draw expected limits plot
 limits->Draw("COLZ TEXT"); 
 // save plot
 CMS_lumi(c,false,0);
 c->cd();
 c->SaveAs(Form("%s/limits2D_2HDM_exp.png",outDir.Data()));
 c->SaveAs(Form("%s/limits2D_2HDM_exp.pdf",outDir.Data()));

 // draw observed limits plot
 obslimits->Draw("COLZ TEXT"); 
 // save plot
 CMS_lumi(c,false,0);
 c->cd();
 c->SaveAs(Form("%s/limits2D_2HDM_obs.png",outDir.Data()));
 c->SaveAs(Form("%s/limits2D_2HDM_obs.pdf",outDir.Data()));

 TLegend* leg = new TLegend(0.7,0.2,0.9,0.4); // (x1,y1,x2,y2)
 leg->SetBorderSize(4);
 leg->SetLineWidth(2);
 //leg->SetFillColor(0);
 //leg->SetFillStyle(0);
 leg->AddEntry(limit300,"m_{A0}=300GeV","pl");
 leg->AddEntry(limit400,"m_{A0}=400GeV","pl");
 leg->AddEntry(limit500,"m_{A0}=500GeV","pl");
 leg->AddEntry(limit600,"m_{A0}=600GeV","pl");
 leg->AddEntry(limit700,"m_{A0}=700GeV","pl");
 leg->AddEntry(limit800,"m_{A0}=800GeV","pl");
 leg->SetTextSize(0.03);

 // draw 1D comparisons --expected
 c->Clear();
 c->SetLogy(1);
 limit300->Draw("APL");
 limit400->Draw("PL SAME");
 limit500->Draw("PL SAME");
 limit600->Draw("PL SAME");
 limit700->Draw("PL SAME");
 limit800->Draw("PL SAME");
 leg->Draw("SAME");
 CMS_lumi(c,false,0);
 c->cd();
 c->SaveAs(Form("%s/limits_comparison_2HDM_exp.png",outDir.Data()));
 c->SaveAs(Form("%s/limits_comparison_2HDM_exp.pdf",outDir.Data()));

 // draw 1D comparisons --observed
 c->Clear();
 c->SetLogy(1);
 limit300_obs->Draw("APL");
 limit400_obs->Draw("PL SAME");
 limit500_obs->Draw("PL SAME");
 limit600_obs->Draw("PL SAME");
 limit700_obs->Draw("PL SAME");
 limit800_obs->Draw("PL SAME");
 leg->Draw("SAME");
 CMS_lumi(c,false,0);
 c->cd();
 c->SaveAs(Form("%s/limits_comparison_2HDM_obs.png",outDir.Data()));
 c->SaveAs(Form("%s/limits_comparison_2HDM_obs.pdf",outDir.Data()));

 delete c;


 // make plot with both expected and observed on same graph
 TCanvas* cboth = new TCanvas("cboth","");
 cboth->cd();
 gStyle->SetOptStat(0);
 gStyle->SetPaintTextFormat("2.1f");

 TPad* p1 = new TPad("p1","",0,0.12,1,0.98);
 p1->Draw();
 p1->cd();

 limits->SetMarkerSize(2);
 limits->Draw("TEXT COLZ SAME"); 
 p1->Update();

 Double_t x1,y1,x2,y2;
 p1->GetRange(x1,y1,x2,y2);

 cboth->cd();
 TPad* p2 = new TPad("p2","",0,0.09,1,0.95);
 p2->SetFillStyle(0);
 p2->SetFillColor(0);
 p2->Draw();
 p2->cd();
 p2->Range(x1,y1,x2,y2);

 obslimits->GetXaxis()->SetTitle("");
 obslimits->GetYaxis()->SetTitle("");
 obslimits->SetTitle("");
 obslimits->GetXaxis()->SetBinLabel(1,"");
 obslimits->GetXaxis()->SetBinLabel(2,"");
 obslimits->GetXaxis()->SetBinLabel(3,"");
 obslimits->GetXaxis()->SetBinLabel(4,"");
 obslimits->GetXaxis()->SetBinLabel(5,"");
 obslimits->GetXaxis()->SetBinLabel(6,"");
 obslimits->GetXaxis()->SetBinLabel(7,"");
 obslimits->GetXaxis()->SetBinLabel(8,"");
 obslimits->GetYaxis()->SetBinLabel(1,"");
 obslimits->GetYaxis()->SetBinLabel(2,"");
 obslimits->GetYaxis()->SetBinLabel(3,"");
 obslimits->GetYaxis()->SetBinLabel(4,"");
 obslimits->GetYaxis()->SetBinLabel(5,"");
 obslimits->GetYaxis()->SetBinLabel(6,"");

 obslimits->Draw("TEXT SAME");
 p2->Update();

 CMS_lumi(cboth,false,0);
 cboth->cd();
 c->SaveAs(Form("%s/limits2D_2HDM_ExpAndObs.png",outDir.Data()));
 c->SaveAs(Form("%s/limits2D_2HDM_ExpAndObs.pdf",outDir.Data()));
 delete cboth;


}

void getXsec(TFile* file, UInt_t mA0, UInt_t mZp, Double_t & xsec){

  TH2F* xsecs = (TH2F*)file->Get("xsec1"); 
  if (xsecs!=(TH2F*)NULL){
     Int_t binX = xsecs->GetXaxis()->FindBin(mZp);
     Int_t binY = xsecs->GetYaxis()->FindBin(mA0);
     xsec = xsecs->GetBinContent(binX,binY); 
  }
  else{
   xsec = 1;
   std::cout << "Couldn't find xsec histogram" << std::endl;
  }
} 


void getLimits(TFile* file, Double_t & Limit, Double_t quantile){

  Double_t limit;
  Float_t quantileExpected;

  TBranch *b_limit;
  TBranch *b_quantileExpected;

  TTree* tree = (TTree*)file->Get("limit");
  if (tree!=(TTree*)NULL){
 
    tree->SetBranchAddress("limit", &limit, &b_limit);
    tree->SetBranchAddress("quantileExpected", &quantileExpected, &b_quantileExpected);
 
    Limit = 0;
    UInt_t nentries = tree->GetEntries();
    for (UInt_t entry = 0; entry < nentries; entry++){
      tree->GetEntry(entry);
      //std::cout << "Quantile = " << quantileExpected << std::endl;
      //std::cout << "Limit    = " << limit << std::endl;
      if (quantileExpected==quantile) Limit=limit;
    }

  }// end valid tree
  else Limit = 0;
  //std::cout << "Limit    = " << Limit << std::endl;
  
  delete tree;

}

