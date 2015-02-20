#include "General.h"


void PlotsTree(TString plots="1btag", int ch=0, TString sys="", bool EvtNorm=false){

  
  TString files  = dirnameIn + fl;  
  
  /****************
      Channel
   ***************/
  
  TString channel;
  if(ch==0) channel="mumu";
  if(ch==1) channel="ee";  
  if(ch==2) channel="mue"; 
  if(ch==3) channel="all"; 
  if(ch==4) channel="mmee"; 
  
  gROOT->SetStyle("Plain");
  gStyle->SetOptFit(1000);
  gStyle->SetOptStat("emruo");
  gStyle->SetOptStat(kFALSE);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);


  /****************
      tdr Style
   ***************/
  //gROOT->ProcessLine(".L /gpfs/csic_projects/cms/brochero/ttbar/TopCodeLegacy/tdrStyle.C");
  gROOT->ProcessLine(".L /home/brochero/ttbar/TopCodeLegacy/tdrStyle.C");
  setTDRStyle();

  Int_t chatch = 1756;
  TColor *color = new TColor(chatch, 0.3, 0.5, 0.5, "", 0.45); // alpha = 0.5
  

  if(plots=="dilepton") dd_dd_uncer=0.0;

  int histos=14;


  /****************
        Data
   ***************/
  DoubleMuon=loadhistograms(plots,files + "_DataMu.root");
  DoubleMuon=addhistograms(DoubleMuon);

  DoubleElectron=loadhistograms(plots,files + "_DataEG.root");
  DoubleElectron=addhistograms(DoubleElectron);

  MuEG=loadhistograms(plots,files + "_DataMuEG.root");
  MuEG=addhistograms(MuEG);

  for(int j=0; j<histos; j++){
    
    Data.hist[j][0]=(TH1F*)DoubleMuon.hist[j][0]->Clone();
    Data.hist[j][1]=(TH1F*)DoubleElectron.hist[j][1]->Clone();
    Data.hist[j][2]=(TH1F*)MuEG.hist[j][2]->Clone();
    
  }

  Data=addhistograms(Data);
  setupdraw(Data, kBlack); 


  /****************
       DY - DD
   ***************/  

  ZDY=loadhistograms(plots,files + "_ZDY.root");
   
  scalehistograms(ZDY, 0, SFmumu); // Scale Factors
  scalehistograms(ZDY, 1, SFee);   // Scale Factors
  scalehistograms(ZDY, 2, SFmue);  // Scale Factors

  ZDY=addhistograms(ZDY); // All Channels
  Z=ZDY;

  setupdraw(Z, kAzure-2);


  /****************
      MC Sample
   ***************/  

  TW=loadhistograms(plots,files + "_TW" + sys + ".root");

  scalehistograms(TW, 0, SFmumu); // Scale Factors
  scalehistograms(TW, 1, SFee);   // Scale Factors
  scalehistograms(TW, 2, SFmue);  // Scale Factors

  TW=addhistograms(TW); // All Channels


  TbarW=loadhistograms(plots,files + "_TbarW" + sys + ".root");

  scalehistograms(TbarW, 0, SFmumu); // Scale Factors
  scalehistograms(TbarW, 1, SFee);   // Scale Factors
  scalehistograms(TbarW, 2, SFmue);  // Scale Factors

  TbarW=addhistograms(TbarW); // All Channels

  for(int j=0; j<histos; j++){
    for(int k=0; k<5; k++){
      
      SingleT.hist[j][k]=(TH1F*)TW.hist[j][k]->Clone();
      SingleT.hist[j][k]->Add(SingleT.hist[j][k],TbarW.hist[j][k]);
      
    }
  }
  

  setupdraw(SingleT, kPink-3); //Select Histogram Color

  ///////////////////////////////////////////////////////////////

  TTbar=loadhistograms(plots,files + "_TTbar_MadSpin" + sys + ".root"); // 7TeV
  //TTbar=loadhistograms(plots,files + "_TTJets_MadSpin" + sys + ".root"); // 8TeV

  scalehistograms(TTbar, 0, SFmumu);
  scalehistograms(TTbar, 1, SFee);
  scalehistograms(TTbar, 2, SFmue);
  
  TTbar=addhistograms(TTbar);
  setupdraw(TTbar, kRed+1);

  //////////////////////////////////////////////////////////////
  
  //TTbarBkg=loadhistograms(plots,files + "_TTJetsMGtauolaBkg.root"); // 8TeV
  TTbarBkg=loadhistograms(plots,files + "_TTbar_MadSpinBkg.root"); // 7TeV

  scalehistograms(TTbarBkg, 0, SFmumu);
  scalehistograms(TTbarBkg, 1, SFee);
  scalehistograms(TTbarBkg, 2, SFmue);

  TTbarBkg=addhistograms(TTbarBkg);

  WJets=loadhistograms(plots,files + "_WJets_Madgraph.root");

  scalehistograms(WJets, 0, SFmumu); // Scale Factors
  scalehistograms(WJets, 1, SFee);   // Scale Factors
  scalehistograms(WJets, 2, SFmue);  // Scale Factors

  WJets=addhistograms(WJets); // All Channels

  for(int j=0; j<histos; j++){
    for(int k=0; k<5; k++){
      
      NONWZ.hist[j][k]=(TH1F*)TTbarBkg.hist[j][k]->Clone();
      //NONWZ.hist[j][k]=(TH1F*)WJets.hist[j][k]->Clone();// Only W+Jets
      NONWZ.hist[j][k]->Add(NONWZ.hist[j][k],WJets.hist[j][k]); // Only ttbar Bkg 
      
    }
  }

  setupdraw(NONWZ, kGreen-3); //Select Histogram Color
  
  //////////////////////////////////////////////////////////////

  WW=loadhistograms(plots,files + "_WW" + sys + ".root");

  scalehistograms(WW, 0, SFmumu); // Scale Factors
  scalehistograms(WW, 1, SFee);   // Scale Factors
  scalehistograms(WW, 2, SFmue);  // Scale Factors

  WW=addhistograms(WW); // All Channels


  WZ=loadhistograms(plots,files + "_WZ" + sys + ".root");

  scalehistograms(WZ, 0, SFmumu); // Scale Factors
  scalehistograms(WZ, 1, SFee);   // Scale Factors
  scalehistograms(WZ, 2, SFmue);  // Scale Factors

  WZ=addhistograms(WZ); // All Channels


  ZZ=loadhistograms(plots,files + "_ZZ" + sys + ".root");

  scalehistograms(ZZ, 0, SFmumu); // Scale Factors
  scalehistograms(ZZ, 1, SFee);   // Scale Factors
  scalehistograms(ZZ, 2, SFmue);  // Scale Factors

  ZZ=addhistograms(ZZ); // All Channels


  for(int j=0; j<histos; j++){
    for(int k=0; k<5; k++){
      
      VV.hist[j][k]=(TH1F*)WW.hist[j][k]->Clone();
      VV.hist[j][k]->Add(VV.hist[j][k],WZ.hist[j][k]);
      VV.hist[j][k]->Add(VV.hist[j][k],ZZ.hist[j][k]);
      
    }
  }
  
  setupdraw(VV, kOrange-3); //Select Histogram Color
  

  /****************
    Normalization
   ***************/  

  if(EvtNorm){

    int NormHisto=5; //Jet Multiplicity
    //int NormHisto=6; //Btag Jet Multiplicity

    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "!!!!!!!  Plots normalized to DATA   !!!!!!" << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

    float EvData_mumu=Data.hist[NormHisto][0]->Integral();
    float EvData_ee  =Data.hist[NormHisto][1]->Integral();
    float EvData_mue =Data.hist[NormHisto][2]->Integral();
    
    float EvMC_mumu=TTbar.hist[NormHisto][0]->Integral() + SingleT.hist[NormHisto][0]->Integral() + VV.hist[NormHisto][0]->Integral();
    float EvMC_ee  =TTbar.hist[NormHisto][1]->Integral() + SingleT.hist[NormHisto][1]->Integral() + VV.hist[NormHisto][1]->Integral();
    float EvMC_mue =TTbar.hist[NormHisto][2]->Integral() + SingleT.hist[NormHisto][2]->Integral() + VV.hist[NormHisto][2]->Integral();
    
    float EvNorm_mumu = (EvData_mumu - (Z.hist[NormHisto][0]->Integral() + NONWZ.hist[NormHisto][0]->Integral()))/EvMC_mumu;
    float EvNorm_ee   = (EvData_ee   - (Z.hist[NormHisto][1]->Integral() + NONWZ.hist[NormHisto][1]->Integral()))/EvMC_ee;
    float EvNorm_mue  = (EvData_mue  - (Z.hist[NormHisto][2]->Integral() + NONWZ.hist[NormHisto][2]->Integral()))/EvMC_mue;
    float EvNorm_mmee = (EvData_mumu + EvData_ee - (Z.hist[NormHisto][0]->Integral() + Z.hist[NormHisto][1]->Integral()) 
			 - (NONWZ.hist[NormHisto][0]->Integral() + NONWZ.hist[NormHisto][1]->Integral()))/(EvMC_mumu + EvMC_ee);
    float EvNorm_all  = (EvData_mumu + EvData_ee + EvData_mue - (Z.hist[NormHisto][0]->Integral() + Z.hist[NormHisto][1]->Integral() + Z.hist[NormHisto][2]->Integral()) - (NONWZ.hist[NormHisto][0]->Integral() + NONWZ.hist[NormHisto][1]->Integral() + NONWZ.hist[NormHisto][2]->Integral()))/(EvMC_mumu + EvMC_ee + EvMC_mue);
    
    
    scalehistograms(TTbar, 0, EvNorm_mumu);
    scalehistograms(TTbar, 1, EvNorm_ee);
    scalehistograms(TTbar, 2, EvNorm_mue);
    scalehistograms(TTbar, 3, EvNorm_all);
    scalehistograms(TTbar, 4, EvNorm_mmee);
    
    scalehistograms(SingleT, 0, EvNorm_mumu);
    scalehistograms(SingleT, 1, EvNorm_ee);
    scalehistograms(SingleT, 2, EvNorm_mue);
    scalehistograms(SingleT, 3, EvNorm_all);
    scalehistograms(SingleT, 4, EvNorm_mmee);
    
    scalehistograms(NONWZ, 0, EvNorm_mumu);
    scalehistograms(NONWZ, 1, EvNorm_ee);
    scalehistograms(NONWZ, 2, EvNorm_mue);
    scalehistograms(NONWZ, 3, EvNorm_all);
    scalehistograms(NONWZ, 4, EvNorm_mmee);
    
    scalehistograms(VV, 0, EvNorm_mumu);
    scalehistograms(VV, 1, EvNorm_ee);
    scalehistograms(VV, 2, EvNorm_mue);
    scalehistograms(VV, 3, EvNorm_all);
    scalehistograms(VV, 4, EvNorm_mmee);
  }  
  
  ////////////////////////////////////////////////////////////////////
  /////////////////////     THStack     //////////////////////////////
  ////////////////////////////////////////////////////////////////////

  for(int j=0; j<histos; j++){
    
    TString variable;

    for(int k=0; k<5; k++){
      MCStack.mc[j][k]=new THStack(variable, "");
      MCStack.mc[j][k]->SetHistogram(TTbar.hist[j][k]);
    }
  }

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  MCStack=addstack(MCStack,TTbar);

  MCStack=addstack(MCStack,Z);

  MCStack=addstack(MCStack,SingleT);

  MCStack=addstack(MCStack,NONWZ);

  MCStack=addstack(MCStack,VV);

  ///////////////////////////////////////////////////////////////////

  TCanvas *cPlots[14];//histos
  
  for(int i=0;i<histos;i++){
    char can[200];
    sprintf(can,"canvas%i",i);
    cPlots[i]=new TCanvas(can,"Preselection Plots");
    cPlots[i]->Divide(1,2);  
  }

  
  TLegend *leg[14];
  TPad    *pad[14][2]; 
  
  for(int i=0; i<histos; i++){

    leg[i]=createlegend(leg[i],plots);//each Plot
    
    //Plot Pad
    pad[i][0] = (TPad*)cPlots[i]->GetPad(1); 
    pad[i][0]->SetPad(0.01, 0.23, 0.99, 0.99);
    pad[i][0]->SetTopMargin(0.1);
    pad[i][0]->SetRightMargin(0.04);
    
    //Ratio Pad
    pad[i][1] = (TPad*)cPlots[i]->GetPad(2);
    pad[i][1]->SetPad(0.01, 0.02, 0.99, 0.3);
    pad[i][1]->SetGridx();
    pad[i][1]->SetGridy();
    pad[i][1]->SetTopMargin(0.05);
    pad[i][1]->SetBottomMargin(0.4);
    pad[i][1]->SetRightMargin(0.04);

    pad[i][0]->cd();
 
    MCStack.mc[i][ch]->Draw("hist");
    MCStack.mc[i][ch]->GetYaxis()->SetTitle("Events");
    MCStack.mc[i][ch]->GetYaxis()->SetTitleOffset(1.2);
    MCStack.mc[i][ch]->GetYaxis()->SetTitleSize(0.07);
    MCStack.mc[i][ch]->GetYaxis()->SetLabelSize(0.055);
    MCStack.mc[i][ch]->GetYaxis()->SetNdivisions(607);
    //MCStack.mc[i][ch]->GetYaxis()->SetLabelSize(0.05);
    TGaxis *hYaxis = (TGaxis*)MCStack.mc[i][ch]->GetYaxis();
    //hYaxis->SetMaxDigits(3);
    MCStack.mc[i][ch]->GetXaxis()->SetLabelSize(0.0);
    MCStack.mc[i][ch]->GetXaxis()->SetTitle("");

    
    // Number of events in the Legend
    // leg[i]=addlegend(leg[i],"Data      ", Data.hist[i][ch]);
    // leg[i]=addlegend(leg[i],"VV        ", VV.hist[i][ch]);
    // leg[i]=addlegend(leg[i],"Non W/Z   ", NONWZ.hist[i][ch]);
    // leg[i]=addlegend(leg[i],"Single Top", SingleT.hist[i][ch]);
    // leg[i]=addlegend(leg[i],"Z/#gamma* l^{+}l^{-}  ", Z.hist[i][ch]);
    // leg[i]=addlegend(leg[i],"t#bar{t}        ", TTbar.hist[i][ch]);
    // leg[i]->Draw("SAME");


    
    float maxh=Data.hist[i][ch]->GetMaximum();
    if(maxh<MCStack.mc[i][ch]->GetMaximum()) maxh=MCStack.mc[i][ch]->GetMaximum();
    MCStack.mc[i][ch]->SetMaximum(1.7*maxh);
    
    /******************************************
     ******************************************
                  Uncertainties
    *******************************************
    *******************************************/

    TH1F *totalmc = (TH1F*) MCStack.mc[i][ch]->GetStack()->Last()->Clone();

    for (int nbin=0; nbin<totalmc->GetNbinsX(); nbin++){


      /***************************
       Theoretical  Uncertainties
      ***************************/

      float ThTTbarUnc=sqrt((TTbar.hist[i][ch]->GetBinContent(nbin+1)*theoryQ2_error)*
			    (TTbar.hist[i][ch]->GetBinContent(nbin+1)*theoryQ2_error)+
			    (TTbar.hist[i][ch]->GetBinContent(nbin+1)*theoryMatching_error)*
			    (TTbar.hist[i][ch]->GetBinContent(nbin+1)*theoryMatching_error)			   
			    );
      
      float ThSingleTUnc=sqrt((SingleT.hist[i][ch]->GetBinContent(nbin+1)*theoryQ2_error)*
			      (SingleT.hist[i][ch]->GetBinContent(nbin+1)*theoryQ2_error)+
			      (SingleT.hist[i][ch]->GetBinContent(nbin+1)*theoryMatching_error)*
			      (SingleT.hist[i][ch]->GetBinContent(nbin+1)*theoryMatching_error)			   
			      );
      
      float ThVVUnc=sqrt((VV.hist[i][ch]->GetBinContent(nbin+1)*theoryQ2_error)*
			 (VV.hist[i][ch]->GetBinContent(nbin+1)*theoryQ2_error)+
			 (VV.hist[i][ch]->GetBinContent(nbin+1)*theoryMatching_error)*
			 (VV.hist[i][ch]->GetBinContent(nbin+1)*theoryMatching_error)			   
			 );

      float ThTotalUnc=sqrt(ThTTbarUnc*ThTTbarUnc + ThSingleTUnc*ThSingleTUnc + ThVVUnc*ThVVUnc);
      
      /***************************
           Other Uncertainties
      ***************************/
    
      float LumUnc=totalmc->GetBinContent(nbin+1)*lumi_error;

      float XsecTTbarUnc=sqrt((TTbar.hist[i][ch]->GetBinContent(nbin+1)*SF_BR_uncer)*(TTbar.hist[i][ch]->GetBinContent(nbin+1)*SF_BR_uncer) + (TTbar.hist[i][ch]->GetBinContent(nbin+1)*XsecTTbar_uncer )*(TTbar.hist[i][ch]->GetBinContent(nbin+1)*XsecTTbar_uncer));
      float XsecSingleTUnc=SingleT.hist[i][ch]->GetBinContent(nbin+1)*XsecTWVV_uncer;
      float XsecVVUnc=VV.hist[i][ch]->GetBinContent(nbin+1)*XsecTWVV_uncer;
      float XsecTotalUnc=sqrt(XsecTTbarUnc*XsecTTbarUnc + XsecSingleTUnc*XsecSingleTUnc + XsecVVUnc*XsecVVUnc);
		      
      float PUUnc=totalmc->GetBinContent(nbin+1)*PU_uncer;

      float SFIDISOUnc=totalmc->GetBinContent(nbin+1)*lept_uncer;

      float DYDDUnc= Z.hist[i][ch]->GetBinContent(nbin+1)*dd_dd_uncer;
		        
      float btagUnc;
      
      if(i==6 && (plots=="1btag" || plots=="MET")){//btag Plot
	for (int nbtagbin = 0; nbtagbin<5; nbtagbin++) SF_uncer[nbtagbin] = 0.;

	// 14/11/2013 (Systematics_2013.ods/btag Sys:ReReco)
	SF_uncer[0] = 0.0;
	SF_uncer[1] = 0.0;
	SF_uncer[2] = 0.0;
	SF_uncer[3] = 0.0;
	SF_uncer[4] = 0.0;

	btagUnc=TTbar.hist[i][ch]->GetBinContent(nbin+1)*SF_uncer[nbin];

      }//if(btag plot)
      else btagUnc=0.0;      

      float EnergyUnc=sqrt((totalmc->GetBinContent(nbin+1)*LES_uncer)*
			   (totalmc->GetBinContent(nbin+1)*LES_uncer) +
			   (totalmc->GetBinContent(nbin+1)*JES_uncer)*
			   (totalmc->GetBinContent(nbin+1)*JES_uncer) +
			   (totalmc->GetBinContent(nbin+1)*JER_uncer)*
			   (totalmc->GetBinContent(nbin+1)*JER_uncer)
			   );

      float PlotUncer=0.0;
      // if (plots=="dilepton") PlotUncer = sqrt(totalmc->GetBinError(nbin+1)*totalmc->GetBinError(nbin+1) + 
      // 					      LumUnc*LumUnc + 
      // 					      PUUnc*PUUnc + 
      // 					      SFIDISOUnc*SFIDISOUnc + 
      // 					      XsecTotalUnc*XsecTotalUnc );
      // if (plots=="2Jets")    PlotUncer = sqrt(totalmc->GetBinError(nbin+1)*totalmc->GetBinError(nbin+1) + 
      // 					      LumUnc*LumUnc + 
      // 					      PUUnc*PUUnc + 
      // 					      SFIDISOUnc*SFIDISOUnc + 
      // 					      XsecTotalUnc*XsecTotalUnc + 
      // 					      DYDDUnc*DYDDUnc);
      // if (plots=="MET")      PlotUncer = sqrt(totalmc->GetBinError(nbin+1)*totalmc->GetBinError(nbin+1) + 
      // 					      LumUnc*LumUnc + 
      // 					      PUUnc*PUUnc + 
      // 					      SFIDISOUnc*SFIDISOUnc + 
      // 					      XsecTotalUnc*XsecTotalUnc + 
      // 					      DYDDUnc*DYDDUnc);
      // if (plots=="1btag" || (plots=="MET" && i==6))    PlotUncer = sqrt(totalmc->GetBinError(nbin+1)*totalmc->GetBinError(nbin+1) + 
      // 									LumUnc*LumUnc + 
      // 									PUUnc*PUUnc + 
      // 									SFIDISOUnc*SFIDISOUnc + 
      // 									XsecTotalUnc*XsecTotalUnc + 
      // 									DYDDUnc*DYDDUnc + 
      // 									EnergyUnc*EnergyUnc + 
      // 									btagUnc*btagUnc);

      if (plots=="dilepton") PlotUncer=totalmc->GetBinError(nbin+1);
      if (plots=="2Jets")    PlotUncer=totalmc->GetBinError(nbin+1);
      if (plots=="MET")      PlotUncer=totalmc->GetBinError(nbin+1);
      if (plots=="1btag" || (plots=="MET" && i==6))    PlotUncer=sqrt(totalmc->GetBinError(nbin+1)*totalmc->GetBinError(nbin+1) + btagUnc*btagUnc);
      
      totalmc->SetBinError(nbin+1,PlotUncer);
    }

    TGraphErrors *thegraph = new TGraphErrors(totalmc);
    thegraph->SetName("thegraph");
    // thegraph->SetFillStyle(3004);
    // thegraph->SetFillColor(1);
    thegraph->SetFillStyle(1001);
    thegraph->SetFillColor(chatch);
    thegraph->SetLineColor(chatch);

    thegraph->Draw("e2SAME");

    Data.hist[i][ch]->Sumw2();
    Data.hist[i][ch]->SetMarkerStyle(20);
    float MarkerDataSize=0.7;
    Data.hist[i][ch]->SetMarkerSize(MarkerDataSize);
    Data.hist[i][ch]->Draw("SAME");

    /***********************
             Ratio
     **********************/    

    //Graph Ratio Clone
    TH1F *Ra;
    Ra=(TH1F*)Data.hist[i][ch]->Clone();
    Ra->Divide(totalmc);
    ratio.hist[i][ch]=Ra;
    
    ratio.hist[i][ch]->SetMarkerStyle(20);
    ratio.hist[i][ch]->SetMarkerSize(MarkerDataSize);
    ratio.hist[i][ch]->SetMarkerColor(1);
    ratio.hist[i][ch]->SetLineColor(1);
    ratio.hist[i][ch]->SetLineWidth(1);
    ratio.hist[i][ch]->SetMaximum(2);
    ratio.hist[i][ch]->SetMinimum(0);
    ratio.hist[i][ch]->SetTitle("");

    ratio.hist[i][ch]->GetYaxis()->SetTitle("Obs/Exp");
    ratio.hist[i][ch]->GetYaxis()->CenterTitle();
    ratio.hist[i][ch]->GetYaxis()->SetTitleOffset(0.45);
    ratio.hist[i][ch]->GetYaxis()->SetTitleSize(0.16);
    ratio.hist[i][ch]->GetYaxis()->SetLabelSize(0.15);
    ratio.hist[i][ch]->GetYaxis()->SetNdivisions(402);
    ratio.hist[i][ch]->GetXaxis()->SetNdivisions(509);
    ratio.hist[i][ch]->GetXaxis()->SetTitleOffset(1.1);
    ratio.hist[i][ch]->GetXaxis()->SetLabelSize(0.20);
    ratio.hist[i][ch]->GetXaxis()->SetTitleSize(0.16);

    ratio.hist[i][ch]->SetMinimum(0.6);
    ratio.hist[i][ch]->SetMaximum(1.4);


    TGraphErrors *thegraphRatio = new TGraphErrors(ratio.hist[i][ch]);
    thegraphRatio->SetFillStyle(1001);
    thegraphRatio->SetFillColor(chatch);
    thegraphRatio->SetName("thegraphRatio");

    //thegraphRatio->Draw("e2SAME");
    for (int nbin=0; nbin<ratio.hist[i][ch]->GetNbinsX(); nbin++) ratio.hist[i][ch]->SetBinError(nbin+1,0.001); 


    leg[i]=addlegend(leg[i],"Data", Data.hist[i][ch]);
    leg[i]=addlegend(leg[i],"VV", VV.hist[i][ch]);
    leg[i]=addlegend(leg[i],"Non W/Z", NONWZ.hist[i][ch]);
    leg[i]=addlegend(leg[i],"Single t", SingleT.hist[i][ch]);
    leg[i]=addlegend(leg[i],"DY", Z.hist[i][ch]);
    leg[i]=addlegend(leg[i],"t#bar{t}", TTbar.hist[i][ch]);
    leg[i]->AddEntry("thegraph","Uncertainty","f");
    leg[i]->Draw("SAME");


    pad[i][1]->cd();

    ratio.hist[i][ch]->Draw();
    thegraphRatio->Draw("e2");
    ratio.hist[i][ch]->Draw("SAME");

    /***********************
           CMS Legend
     **********************/
    cPlots[i]->cd();
    pad[i][0]->cd();
    if(plots=="dilepton" && (i==3 || i==6) ) pad[i][0]->SetLogy();

    TString htitleCMSChannel;
    if(ch==0) htitleCMSChannel="#mu^{+}#mu^{-} channel";
    if(ch==1) htitleCMSChannel="e^{+}e^{-} channel";
    if(ch==2) htitleCMSChannel="e^{#pm}#mu^{#mp} channel";
    if(ch==3) htitleCMSChannel="Sum of all channels";
    if(ch==4) htitleCMSChannel="ee#mu#mu channels";

    titlePr  = new TLatex(-20.,50.,"Preliminary");
    titlePr->SetNDC();
    titlePr->SetTextAlign(12);
    titlePr->SetX(0.25);
    titlePr->SetY(0.93);
    titlePr->SetTextColor(2);
    titlePr->SetTextFont(42);
    titlePr->SetTextSize(0.05);
    titlePr->SetTextSizePixels(24);
    titlePr->Draw("SAME");

    title  = new TLatex(-20.,50.,"CMS #sqrt{s} = 7TeV, L = 5.1 fb^{-1}");
    //title  = new TLatex(-20.,50.,"CMS #sqrt{s} = 8TeV, L = 19.5 fb^{-1}");
    title->SetNDC();
    title->SetTextAlign(12);
    title->SetX(0.20);
    title->SetY(0.83);
    title->SetTextFont(42);
    title->SetTextSize(0.05);
    title->SetTextSizePixels(24);
    title->Draw("SAME");

    chtitle  = new TLatex(-20.,50.,htitleCMSChannel);
    chtitle->SetNDC();
    chtitle->SetTextAlign(12);
    chtitle->SetX(0.20);
    chtitle->SetY(0.75);
    chtitle->SetTextFont(42);
    chtitle->SetTextSize(0.05);
    chtitle->SetTextSizePixels(24);
    chtitle->Draw("SAME");

    TString dirfigname_pdf=dirnameIn + "figures/pdf/";
    // make a dir if it does not exist!!
    gSystem->mkdir(dirfigname_pdf,       kTRUE);

    TString dirfigname_png=dirnameIn + "figures/png/";
    // make a dir if it does not exist!!
    gSystem->mkdir(dirfigname_png,       kTRUE);

    char fig[200];
    sprintf(fig,"f_%s",TTbar.hist[i][ch]->GetName());

    if(ch==3){ // Change the name!
      for(int c=0;c<199;c++){
	if(fig[c]=='m' && fig[c+1]=='u'){ 
	  fig[c]='c';	
	  fig[c+1]='a';	
	  fig[c+2]='l';	
	  fig[c+3]='l';	
	}
      }
    }//if(channel=all)

    if(ch==4){ // Change the name!
      for(int c=0;c<199;c++){
	if(fig[c]=='m' && fig[c+1]=='u'){ 
	  fig[c+1]='m';	
	  fig[c+2]='e';	
	  fig[c+3]='e';	
	}
      }
    }//if(channel=mmee)

    // PDF
    dirfigname_pdf=dirfigname_pdf + fig;
    cPlots[i]->SaveAs(dirfigname_pdf + ".pdf");

    // PNG
    dirfigname_png=dirfigname_png + fig;
    cPlots[i]->SaveAs(dirfigname_png + ".png");

  }//for(histograms)

}

/////////////////  Functions  //////////////////////

TLegend* createlegend(TLegend *leg, TString plots=""){

  float legx1=0.68; 
  float legy1=0.58;
  float legx2=0.90;
  float legy2=0.89;

  //if(plots=="dilepton") legx1=0.75;

  leg = new TLegend(legx1,legy1,legx2,legy2);
  //leg->SetTextAlign(32);
  leg->SetFillColor(0);
  leg->SetLineColor(1);
  //leg->SetLineWidth(4);
  //leg->SetTextFont(62); 
  leg->SetTextFont(102); // Events in the leg!
  leg->SetTextSize(0.04);

  return leg;
}

TLegend* addlegend(TLegend *leg, TString name, TH1* histo){

  //Number of events
  /////////////////////////////////////////////
  char com[200]; 
  TString test;
  sprintf(com,"%i",(histo->Integral()));

  int cn=strlen(com);
  int hn=name.Sizeof();
  if (name.Contains("t#")) hn=3;
  cn=14-(hn+cn); // 14 comming from the max number of character.
  for(int i=0;i<cn;i++){
    name=name + " ";
  }
  
  name=name+com;
  ////////////////////////////////////////////

  if(name.Contains("Data") || name.Contains("STop")) leg->AddEntry(histo,name,"PL");
  else leg->AddEntry(histo,name,"F");

  return leg;

}

Histograms addhistogram(Histograms histo, Histograms histoIn){
  
  int histos=14;

    for(int j=0; j<histos; j++){
      for(int k=0; k<5; k++){
	histo.hist[j][k]->Add(histo.hist[j][k],histoIn.hist[j][k]);	
      }
    }

  return histo;

}

Histograms addstack(Histograms stack, Histograms histoIn){

  int histos=14;
    for(int j=0; j<histos; j++){
      for(int k=0; k<5; k++){
	stack.mc[j][k]->Add(histoIn.hist[j][k]);	
      }
    }

  return stack;
}

void setupdraw(Histograms h, int color) {

  int histos=14;
  int bin[14];

  for(int j=0; j<histos; j++){
    bin[j]=1;//histos
  }
  
  bin[2]=1; //DeltaPhiLeptons Rebining
  bin[3]=2; //InvMass Rebining

  for(int j=0; j<histos; j++){
    for(int k=0; k<5; k++){
      
      h.hist[j][k]->Rebin(bin[j]);
      
      TString htitle=h.hist[j][k]->GetTitle();

      if(k==0) htitle.Resize(htitle.Sizeof()-8);
      if(k==1) htitle.Resize(htitle.Sizeof()-4);
      if(k==2) htitle.Resize(htitle.Sizeof()-6);
      

      if(htitle.Contains("multiplicity") || htitle.Contains("PV")) h.hist[j][k]->GetXaxis()->SetTitle(htitle);
      else if(htitle.Contains("#Phi") || htitle.Contains("#phi")) h.hist[j][k]->GetXaxis()->SetTitle("|" + htitle + "| [rad]"); 
      else h.hist[j][k]->GetXaxis()->SetTitle(htitle + " [GeV]");

      h.hist[j][k]->SetLineColor(1);
      h.hist[j][k]->SetFillColor(color);
      h.hist[j][k]->SetFillStyle(1001);

      h.hist[j][k]->SetBinContent(h.hist[j][k]->GetNbinsX(),
				     (h.hist[j][k]->GetBinContent(h.hist[j][k]->GetNbinsX()+1)+h.hist[j][k]->GetBinContent(h.hist[j][k]->GetNbinsX())));
      h.hist[j][k]->SetBinContent(h.hist[j][k]->GetNbinsX()+1,0);

      if(j==6) h.hist[6][k]->GetXaxis()->SetTitle("b-jet multiplicity");

    }
  }
  
  
}

Histograms addhistograms(Histograms histoIn){
  
  int histos=14;
  
  for(int j=0; j<histos; j++){
    histoIn.hist[j][3]=(TH1F*)histoIn.hist[j][0]->Clone();
    histoIn.hist[j][4]=(TH1F*)histoIn.hist[j][0]->Clone();
    
    TString htitle=histoIn.hist[j][0]->GetTitle();
    htitle.Resize(htitle.Sizeof()-8);

    histoIn.hist[j][3]->SetTitle(htitle);
    histoIn.hist[j][3]->Add(histoIn.hist[j][3],histoIn.hist[j][1]);
    histoIn.hist[j][3]->Add(histoIn.hist[j][3],histoIn.hist[j][2]);

    histoIn.hist[j][4]->SetTitle(htitle);
    histoIn.hist[j][4]->Add(histoIn.hist[j][4],histoIn.hist[j][1]);
  }

  return histoIn;
}

void scalehistograms(Histograms histoIn, int channel, float SF){
  
  int histos=14;
  
    for(int j=0; j<histos; j++){
      histoIn.hist[j][channel]->Scale(SF);
    }
    
}


Histograms loadhistograms(TString plots,TString namefile){
  
  Histograms histofile;
  
  TFile *file=NULL;//new TFile(namefile);
  
  file = TFile::Open(namefile);
  cout << "loading " << plots << " " << namefile << endl; 
  
  TString channel[3];
  channel[0]="mumu";
  channel[1]="ee";
  channel[2]="mue";
  
  for(int ch=0;ch<3;ch++){

    histofile.hist[0][ch]=(TH1F*)file->Get("hPV_"+channel[ch]+"_"+plots);

    histofile.hist[1][ch]=(TH1F*)file->Get("hMET_"+channel[ch]+"_"+plots);
    histofile.hist[2][ch]=(TH1F*)file->Get("hDelLepPhi_"+channel[ch]+"_"+plots);

    histofile.hist[3][ch]=(TH1F*)file->Get("hInvMass_"+channel[ch]+"_"+plots);
    histofile.hist[4][ch]=(TH1F*)file->Get("hDiLepPt_"+channel[ch]+"_"+plots);

    histofile.hist[5][ch]=(TH1F*)file->Get("hNJets_"+channel[ch]+"_"+plots);
    histofile.hist[6][ch]=(TH1F*)file->Get("hNBtagJets_"+channel[ch]+"_"+plots);    
    histofile.hist[7][ch]=(TH1F*)file->Get("hJet0Pt_"+channel[ch]+"_"+plots);
    histofile.hist[8][ch]=(TH1F*)file->Get("hJet1Pt_"+channel[ch]+"_"+plots);
    histofile.hist[9][ch]=(TH1F*)file->Get("hBtagJet0Pt_"+channel[ch]+"_"+plots);

    histofile.hist[10][ch]=(TH1F*)file->Get("hLep0Pt_"+channel[ch]+"_"+plots);
    histofile.hist[11][ch]=(TH1F*)file->Get("hLep1Pt_"+channel[ch]+"_"+plots);

    histofile.hist[12][ch]=(TH1F*)file->Get("hLep0Phi_"+channel[ch]+"_"+plots);
    histofile.hist[13][ch]=(TH1F*)file->Get("hLep1Phi_"+channel[ch]+"_"+plots);

  }

  return histofile;

  file.Close();

}

