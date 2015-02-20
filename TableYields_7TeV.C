#include <algorithm> // max(a,b)

//#include "/home/brochero/ttbar/TopCodeLegacy/TopResults/YieldsNonWZ-DD.h" //Yields to estimate Non-W/Z
//#include "/home/brochero/ttbar/TopCodeLegacy/TopResults/Yields7TeV_v2.h" //Yields
//#include "/home/brochero/ttbar/TopCodeLegacy/TopResults/Yields7TeV_v3_pTReweight.h" //Yields pT reweight
#include "/home/brochero/ttbar/TopCodeLegacy/TopResults/Yields7TeV_v4.h" //Yields pT reweight
#include "/home/brochero/ttbar/TopCodeLegacy/TopResults/Syst_Yields7TeV_v1.h" //Systematic Yields


void TableYields_7TeV(){

  TableYields_7TeV("dilepton");
  TableYields_7TeV("2Jets");
  TableYields_7TeV("MET");
  TableYields_7TeV("1btag");

}

void TableYields_7TeV(TString cutname, TString energy="7TeV"){

  int cut;
  if(cutname=="dilepton")      cut=0;
  else if(cutname=="2Jets")    cut=1;
  else if(cutname=="MET")      cut=2;
  else if(cutname=="1btag")    cut=3;

  else{
    cout << "Invalid cut name!!!" << endl;
    break; 
  }

  double signalSF[3];
  double err_stat_signalSF[3];
  
  double DY[3];
  double err_DY[3];
  
  double Non_WZ[3];
  double err_Non_WZ[3];

  double Non_WZ_DD[3];
  double err_Non_WZ_DD[3];
  
  double VV[3];
  double err_stat_VV[3];
  
  double Stop[3];
  double err_stat_Stop[3];
  
  double data[3];
  
  // Yields
  for(int ch=0; ch<3; ch++){
    // Signal
    signalSF[ch]          = TTbar_MadSpin[ch][cut];
    err_stat_signalSF[ch] = err_TTbar_MadSpin[ch][cut];
        
    DY[ch]                = ZDY[ch][cut];
    err_DY[ch]            = 0.3*ZDY[ch][cut]; // 30% of Uncertainty in the DD 
    
    Non_WZ[ch]            = TTbar_MadSpinBkg[ch][cut] + WJets_Madgraph[ch][cut];
    err_Non_WZ[ch]        = sqrt(TTbar_MadSpinBkg[ch][cut]**2 + WJets_Madgraph[ch][cut]**2);

    if(ch==2) Non_WZ_DD[ch]         = 67.5;
    else      Non_WZ_DD[ch]         = Non_WZ[ch];
    err_Non_WZ_DD[ch]     = err_Non_WZ[ch];
    
    VV[ch]                = WW[ch][cut] + WZ[ch][cut] + ZZ[ch][cut];
    err_stat_VV[ch]       = sqrt(err_WW[ch][cut]**2 + err_WZ[ch][cut]**2 + err_ZZ[ch][cut]**2);
    
    Stop[ch]              = TW[ch][cut] + TbarW[ch][cut];
    err_stat_Stop[ch]     = sqrt(err_TW[ch][cut]**2 + err_TbarW[ch][cut]**2);

  }
  
  // Data yields
  data[0]  = DataMu  [0][cut];
  data[1]  = DataEG  [1][cut];
  data[2]  = DataMuEG[2][cut];    
  
  
  // Systematics 
  float sig_Up  [3][13];   // [Channel][Syst]
  float sig_Down[3][13]; 
  float TW_Up   [3][13];   
  float TW_Down [3][13]; 
  float VV_Up   [3][13];   
  float VV_Down [3][13]; 
  
  for(int ch=0; ch<3; ch++){
    for(int syst=0; syst<13; syst++){
      sig_Up[ch][syst]   =0.0;
      sig_Down[ch][syst] =0.0;
      
      TW_Up[ch][syst]   =0.0;
      TW_Down[ch][syst] =0.0;
      
      VV_Up[ch][syst]   =0.0;
      VV_Down[ch][syst] =0.0;
    }
  }
  
  for(int ch=0; ch<3; ch++){
    
    // Trigger Efficiency  
    sig_Up[ch][0]   = fabs(TTbar_MadSpin_Tr_Central[ch][cut] - TTbar_MadSpin_Tr_Up[ch][cut]) / TTbar_MadSpin_Tr_Central[ch][cut];
    sig_Down[ch][0] = fabs(TTbar_MadSpin_Tr_Central[ch][cut] - TTbar_MadSpin_Tr_Down[ch][cut]) / TTbar_MadSpin_Tr_Central[ch][cut];
    
    // Lepton Efficiency  
    sig_Up[ch][1]   = fabs(TTbar_MadSpin_IDISO_Central[ch][cut] - TTbar_MadSpin_IDISO_Up[ch][cut]) / TTbar_MadSpin_IDISO_Central[ch][cut];
    sig_Down[ch][1] = fabs(TTbar_MadSpin_IDISO_Central[ch][cut] - TTbar_MadSpin_IDISO_Down[ch][cut]) / TTbar_MadSpin_IDISO_Central[ch][cut];
    
    // Lepton energy Scale (LES)   
    sig_Up[ch][2]   = fabs(TTbar_MadSpin_Central[ch][cut] - TTbar_MadSpin_LES_Up[ch][cut]) / TTbar_MadSpin_Central[ch][cut];
    sig_Down[ch][2] = fabs(TTbar_MadSpin_Central[ch][cut] - TTbar_MadSpin_LES_Down[ch][cut]) / TTbar_MadSpin_Central[ch][cut];
    
    TW_Up[ch][2]    = fabs((TW_Central[ch][cut] + TbarW_Central[ch][cut]) -
    			   (TW_LES_Up[ch][cut] + TbarW_LES_Up[ch][cut])) / (TW_Central[ch][cut] + TbarW_Central[ch][cut]);
    TW_Down[ch][2]  = fabs((TW_Central[ch][cut] + TbarW_Central[ch][cut]) -
    			   (TW_LES_Down[ch][cut] + TbarW_LES_Down[ch][cut])) / (TW_Central[ch][cut] + TbarW_Central[ch][cut]);
    
    VV_Up[ch][2]    = fabs((WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]) -
    			   (WW_LES_Up[ch][cut] + WZ_LES_Up[ch][cut] + ZZ_LES_Up[ch][cut])) / (WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]);
    VV_Down[ch][2]    = fabs((WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]) -
    			     (WW_LES_Down[ch][cut] + WZ_LES_Down[ch][cut] + ZZ_LES_Down[ch][cut]))/ (WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]);
    
    // Jet Energy Scale (JES)
    sig_Up[ch][3]   = fabs(TTbar_MadSpin_Central[ch][cut] - TTbar_MadSpin_JES_Up[ch][cut]) / TTbar_MadSpin_Central[ch][cut];
    sig_Down[ch][3] = fabs(TTbar_MadSpin_Central[ch][cut] - TTbar_MadSpin_JES_Down[ch][cut]) / TTbar_MadSpin_Central[ch][cut];
    
    TW_Up[ch][3]    = fabs((TW_Central[ch][cut] + TbarW_Central[ch][cut]) -
    			   (TW_JES_Up[ch][cut] + TbarW_JES_Up[ch][cut])) / (TW_Central[ch][cut] + TbarW_Central[ch][cut]);
    TW_Down[ch][3]  = fabs((TW_Central[ch][cut] + TbarW_Central[ch][cut]) -
    			   (TW_JES_Down[ch][cut] + TbarW_JES_Down[ch][cut])) / (TW_Central[ch][cut] + TbarW_Central[ch][cut]);
    
    VV_Up[ch][3]    = fabs((WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]) -
    			   (WW_JES_Up[ch][cut] + WZ_JES_Up[ch][cut] + ZZ_JES_Up[ch][cut])) / (WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]);
    VV_Down[ch][3]    = fabs((WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]) -
    			     (WW_JES_Down[ch][cut] + WZ_JES_Down[ch][cut] + ZZ_JES_Down[ch][cut])) / (WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]);
    
    // Jet Energy Resolution (JER)
    sig_Up[ch][4]   = fabs(TTbar_MadSpin_JER_Nom[ch][cut] - TTbar_MadSpin_JER_Up[ch][cut]) / TTbar_MadSpin_JER_Nom[ch][cut];
    sig_Down[ch][4] = fabs(TTbar_MadSpin_JER_Nom[ch][cut] - TTbar_MadSpin_JER_Down[ch][cut]) / TTbar_MadSpin_JER_Nom[ch][cut];
    
    TW_Up[ch][4]    = fabs((TW_Central[ch][cut] + TbarW_Central[ch][cut]) -
    			   (TW_JER_Up[ch][cut] + TbarW_JER_Up[ch][cut])) / (TW_Central[ch][cut] + TbarW_Central[ch][cut]);
    TW_Down[ch][4]  = fabs((TW_Central[ch][cut] + TbarW_Central[ch][cut]) -
    			   (TW_JER_Down[ch][cut] + TbarW_JER_Down[ch][cut])) / (TW_Central[ch][cut] + TbarW_Central[ch][cut]);
    VV_Up[ch][4]    = fabs((WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]) -
    			   (WW_JER_Up[ch][cut] + WZ_JER_Up[ch][cut] + ZZ_JER_Up[ch][cut])) / (WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]);
    VV_Down[ch][4]    = fabs((WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]) -
    			     (WW_JER_Down[ch][cut] + WZ_JER_Down[ch][cut] + ZZ_JER_Down[ch][cut])) / (WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]);
    
    // b-jet tagging
    sig_Up[ch][5]   = fabs(TTbar_MadSpin_Central[ch][cut] - TTbar_MadSpin_btag_Up[ch][cut]) / TTbar_MadSpin_Central[ch][cut];
    sig_Down[ch][5] = fabs(TTbar_MadSpin_Central[ch][cut] - TTbar_MadSpin_btag_Down[ch][cut]) / TTbar_MadSpin_Central[ch][cut];
    
    TW_Up[ch][5]    = fabs((TW_Central[ch][cut] + TbarW_Central[ch][cut]) -
    			   (TW_btag_Up[ch][cut] + TbarW_btag_Up[ch][cut])) / (TW_Central[ch][cut] + TbarW_Central[ch][cut]);
    TW_Down[ch][5]  = fabs((TW_Central[ch][cut] + TbarW_Central[ch][cut]) -
    			   (TW_btag_Down[ch][cut] + TbarW_btag_Down[ch][cut])) / (TW_Central[ch][cut] + TbarW_Central[ch][cut]);
    
    VV_Up[ch][5]    = fabs((WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]) -
    			   (WW_btag_Up[ch][cut] + WZ_btag_Up[ch][cut] + ZZ_btag_Up[ch][cut])) / (WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]);
    VV_Down[ch][5]    = fabs((WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]) -
    			     (WW_btag_Down[ch][cut] + WZ_btag_Down[ch][cut] + ZZ_btag_Down[ch][cut])) / (WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]);
    

    // PileUp (PU)
    sig_Up[ch][6]   = fabs(TTbar_MadSpin_Central[ch][cut] - TTbar_MadSpin_PU_Up[ch][cut]) / TTbar_MadSpin_Central[ch][cut];
    sig_Down[ch][6] = fabs(TTbar_MadSpin_Central[ch][cut] - TTbar_MadSpin_PU_Down[ch][cut]) / TTbar_MadSpin_Central[ch][cut];
    
    TW_Up[ch][6]    = fabs((TW_Central[ch][cut] + TbarW_Central[ch][cut]) -
    			   (TW_PU_Up[ch][cut] + TbarW_PU_Up[ch][cut])) / (TW_Central[ch][cut] + TbarW_Central[ch][cut]);
    TW_Down[ch][6]  = fabs((TW_Central[ch][cut] + TbarW_Central[ch][cut]) -
    			   (TW_PU_Down[ch][cut] + TbarW_PU_Down[ch][cut])) / (TW_Central[ch][cut] + TbarW_Central[ch][cut]);
    
    VV_Up[ch][6]    = fabs((WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]) -
    			   (WW_PU_Up[ch][cut] + WZ_PU_Up[ch][cut] + ZZ_PU_Up[ch][cut])) / (WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]);
    VV_Down[ch][6]    = fabs((WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]) -
    			     (WW_PU_Down[ch][cut] + WZ_PU_Down[ch][cut] + ZZ_PU_Down[ch][cut])) / (WW_Central[ch][cut] + WZ_Central[ch][cut] + ZZ_Central[ch][cut]);

    // Scale
    sig_Up[ch][7]   = fabs(TTbar_MadSpin_Central[ch][cut] - TTbar_MadSpin_Scale_Up[ch][cut]) / TTbar_MadSpin_Central[ch][cut];
    sig_Down[ch][7] = fabs(TTbar_MadSpin_Central[ch][cut] - TTbar_MadSpin_Scale_Down[ch][cut]) / TTbar_MadSpin_Central[ch][cut];
    
    // Matching Parton to Showers
    sig_Up[ch][8]   = fabs(TTbar_MadSpin_Central[ch][cut] - TTbar_MadSpin_Matching_Up[ch][cut]) / TTbar_MadSpin_Central[ch][cut];
    sig_Down[ch][8] = fabs(TTbar_MadSpin_Central[ch][cut] - TTbar_MadSpin_Matching_Down[ch][cut]) / TTbar_MadSpin_Central[ch][cut];
    
    // Experimental Xsec
    // A Br correction is applied to MC in TreeReader.C -> (0.108*9)*(0.108*9); with 2*0.0009/0.108~1.7% uncertainty.
    sig_Up[ch][9]   = 0.0;
    sig_Down[ch][9] = 0.0;

    TW_Up[ch][9]    = 0.2;
    TW_Down[ch][9]  = 0.2;
    VV_Up[ch][9]    = 0.2;
    VV_Down[ch][9]  = 0.2;
			    
    // // Drell-Yan
    
    // // Non-W/Z
    
    
  }// for(ch) 
  
  float sig_Unc [3][13];   // [Channel][Syst]
  float TW_Unc  [3][13]; 
  float VV_Unc  [3][13]; 
  
  for(int ch=0; ch<3; ch++){
    for(int syst=0; syst<13; syst++){
      
      if (ch==2 && syst==1) sig_Unc[ch][syst] = 0.6 * max(sig_Up[ch][syst] , sig_Down[ch][syst]); // Temporal Solution SF_IDISO_mue
      else sig_Unc[ch][syst] = max(sig_Up[ch][syst] , sig_Down[ch][syst]);
      TW_Unc [ch][syst] = max(TW_Up[ch][syst]  , TW_Down[ch][syst]);
      VV_Unc [ch][syst] = max(VV_Up[ch][syst]  , VV_Down[ch][syst]);
      
    }
  }
  
  ///////////////////
  // Cross Section // 
  ///////////////////

  if (energy=="8TeV"){
    float luminosity   = 19468.3;// pb-1
    float SysLumi      = 0.026;  // 2.6%
    double xsec_theory = 245.8;  // pb (8 TeV)
  }
  else if (energy=="7TeV"){
    cout << "7TeV!!" << endl;
    float luminosity   = 5100.0;// pb-1
    float SysLumi      = 0.022; // 2.2%
    double xsec_theory = 177.31;  // pb (7 TeV)
  }
  

  float err_syst_signalSF[3] = {0.0,0.0,0.0};  
  float err_lumi_signalSF[3] = {0.0,0.0,0.0};  

  float err_Stop[3]      = {0.0,0.0,0.0};  
  float err_syst_Stop[3] = {0.0,0.0,0.0};  
  float err_lumi_Stop[3] = {0.0,0.0,0.0};  

  float err_VV[3]      = {0.0,0.0,0.0};  
  float err_syst_VV[3] = {0.0,0.0,0.0};  
  float err_lumi_VV[3] = {0.0,0.0,0.0};  

  float backg[3]     = {0.0,0.0,0.0};
  float err_backg[3] = {0.0,0.0,0.0};

  float xsec[3]          = {0.0,0.0,0.0};
  float err_stat_xsec[3] = {0.0,0.0,0.0};
  float err_syst_xsec[3] = {0.0,0.0,0.0};
  float err_lumi_xsec[3] = {0.0,0.0,0.0};
  float err_syst_xsec_cont[3][16];  // Uncertainties in the Xsec

  float acceptance[3]          = {0.0,0.0,0.0};
  float err_stat_acceptance[3] = {0.0,0.0,0.0};
  float err_syst_acceptance[3] = {0.0,0.0,0.0};

  for (Int_t ch=0; ch<3; ch++) {

    for(int syst=0; syst<16; syst++) err_syst_xsec_cont[ch][syst]=0.0;

    for(int syst=0; syst<13; syst++){
      err_syst_signalSF[ch]+ = sig_Unc[ch][syst]**2;
      err_syst_Stop[ch]+     = TW_Unc[ch][syst]**2;
      err_syst_VV[ch]+       = VV_Unc[ch][syst]**2;
    }

    // Signal
    err_syst_signalSF[ch] = signalSF[ch]*sqrt(err_syst_signalSF[ch]);
    err_lumi_signalSF[ch] = signalSF[ch]*SysLumi;

    acceptance[ch]          = signalSF[ch] / (luminosity * xsec_theory);
    err_stat_acceptance[ch] = err_stat_signalSF[ch] / (luminosity * xsec_theory);
    err_syst_acceptance[ch] = err_syst_signalSF[ch] / (luminosity * xsec_theory);
    
    // Background
    err_syst_Stop[ch] = Stop[ch]*sqrt(err_syst_Stop[ch]);
    err_lumi_Stop[ch] = Stop[ch]*SysLumi;
    
    err_syst_VV[ch]   = VV[ch]*sqrt(err_syst_VV[ch]);
    err_lumi_VV[ch]   = VV[ch]*SysLumi;
    

    err_Stop[ch] = sqrt(err_syst_Stop[ch]**2 + err_stat_Stop[ch]**2 + err_lumi_Stop[ch]**2);
    err_VV[ch]   = sqrt(err_syst_VV[ch]**2 + err_stat_VV[ch]**2 + err_lumi_VV[ch]**2);

    
    // Total background: VV + Stop + DY + Fakes  
    backg[ch] = VV[ch] + Stop[ch] + DY[ch] + Non_WZ_DD[ch];
    err_backg[ch] = sqrt(err_VV[ch]**2 + err_Stop[ch]**2 + err_DY[ch]**2 + err_Non_WZ_DD[ch]**2); 
  
    xsec[ch] = (xsec_theory/signalSF[ch]) * (data[ch] - backg[ch]);  

    err_stat_xsec[ch] = sqrt(data[ch])*(xsec[ch]/(data[ch] - backg[ch]));
    err_syst_xsec[ch] = xsec[ch] * sqrt( (err_backg[ch]/(data[ch] - backg[ch]))**2 + (err_syst_signalSF[ch]/signalSF[ch])**2 ); 
    err_lumi_xsec[ch] = xsec[ch] * SysLumi;


    // break-down of the systematic uncertainty in pb per channel
    for(int syst=0; syst<13; syst++) err_syst_xsec_cont[ch][syst] = xsec[ch]*sig_Unc[ch][syst];
    err_syst_xsec_cont[ch][10] = xsec[ch]*err_DY[ch]   / (data[ch] - backg[ch]); // DY-DD
    err_syst_xsec_cont[ch][13] = xsec[ch]*err_Stop[ch] / (data[ch] - backg[ch]); // tW
    err_syst_xsec_cont[ch][14] = xsec[ch]*err_VV[ch]   / (data[ch] - backg[ch]); // VV

    // Total Systematic Uncertainty
    for(int syst=0; syst<15; syst++) err_syst_xsec_cont[ch][15]+ = err_syst_xsec_cont[ch][syst]**2;
    err_syst_xsec_cont[ch][15] = sqrt(err_syst_xsec_cont[ch][15]);

  }// for(ch)

  cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
  cout << "---------------------------------------------------------" << endl;
  cout << cutname << endl;
  cout << "Xsec_mumu = " << xsec[0] << " +/- " << err_stat_xsec[0] << " +/- " << err_syst_xsec[0] << " +/- " << err_lumi_xsec[0] <<endl;
  cout << "Xsec_ee = "   << xsec[1] << " +/- " << err_stat_xsec[1] << " +/- " << err_syst_xsec[1] << " +/- " << err_lumi_xsec[1] <<endl;
  cout << "Xsec_mue = "  << xsec[2] << " +/- " << err_stat_xsec[2] << " +/- " << err_syst_xsec[2] << " +/- " << err_lumi_xsec[2] <<endl;
  cout << "---------------------------------------------------------" << endl;
  cout << "Systematics (mumu):" << endl;
  cout << "Signal= "    << 100*err_syst_signalSF[0]/signalSF[0] 
       << " %  TW/TbarW= " << 100*err_syst_Stop[0]/Stop[0] 
       << " %  VV= "       << 100*err_syst_VV[0]/VV[0] 
       << " %" << endl;
  cout << "---------------------------------------------------------" << endl;
  cout << "Systematics (ee):" << endl;
  cout << "Signal= "    << 100*err_syst_signalSF[1]/signalSF[1] 
       << " %  TW/TbarW= " << 100*err_syst_Stop[1]/Stop[1] 
       << " %  VV= "       << 100*err_syst_VV[1]/VV[1] 
       << " %" << endl;
  cout << "---------------------------------------------------------" << endl;
  cout << "Systematics (mue):" << endl;
  cout << "Signal= "    << 100*err_syst_signalSF[2]/signalSF[2] 
       << " %  TW/TbarW= " << 100*err_syst_Stop[2]/Stop[2] 
       << " %  VV= "       << 100*err_syst_VV[2]/VV[2] 
       << " %" << endl;
  cout << "---------------------------------------------------------" << endl;
  cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;

  ///////////////////
  //    Tables     // 
  ///////////////////


  //TString dirtexname="TopResults/tex_7TeV_v2/";
  //TString dirtexname="TopResults/tex_NonWZ-DD/";
  //TString dirtexname="TopResults/tex_7TeV_v3_pTReweight_NonWZ-DD/";
  TString dirtexname="TopResults/tex_7TeV_v4_NonWZ-DD/";
  // make a dir if it does not exist!!
  gSystem->mkdir(dirtexname,       kTRUE);

  TString Xsectexfile=dirtexname + "Xsec" + cutname + ".tex";

  FILE* fxsecout = fopen(Xsectexfile, "w");

  
  fprintf(fxsecout,"\\begin\{tabular\}\{\|l\|c\|c\|\}\\hline\n");
  fprintf(fxsecout,"Channel \& \$\\sigma_{\\ttbar}\$ \& Acceptance (\\%) \\\\\\hline\\hline\n");
  fprintf(fxsecout,"\$\{\\mu\\mu}\$ \& \$ %.1f \\pm \%.1f \\pm %.1f \\pm %.1f \$ \& %.3f \\\\\\hline\n", xsec[0], err_stat_xsec[0], err_syst_xsec[0], err_lumi_xsec[0], acceptance[0]*100. );
  fprintf(fxsecout,"\$\{\\ee}\$ \& \$ %.1f \\pm \%.1f \\pm %.1f \\pm %.1f \$ \& %.3f \\\\\\hline\n", xsec[1], err_stat_xsec[1], err_syst_xsec[1], err_lumi_xsec[1], acceptance[1]*100. );
  fprintf(fxsecout,"\$\{\\mue}\$ \& \$ %.1f \\pm \%.1f \\pm %.1f \\pm %.1f \$ \& %.3f \\\\\\hline\n", xsec[2], err_stat_xsec[2], err_syst_xsec[2], err_lumi_xsec[2], acceptance[2]*100. );

  fprintf(fxsecout,"\\end\{tabular\}\n");

  fclose(fxsecout);


  TString texfile=dirtexname + "Yields" + cutname + ".tex";

  FILE* fyout = fopen(texfile, "w");

  TString chname[3];
  chname[0]="$\\mu\\mu$";
  chname[1]="$ee$";
  chname[2]="$e\\mu$";
  
  fprintf(fyout,"\{\\tiny\n");
  fprintf(fyout,"\\begin\{tabular\}\{\|c\|c\|c\|c\|c\|c\|c\|c\|\}\\hline\n");
  fprintf(fyout," %s & Data & Total MC & \$ t\\bar\{t\}\_\{Sig\} \$ & \$ Z+Jets \$ & \$ tW \$ & \$ VV \$ & Non W/Z \\\\\\hline\\hline\n",cutname.Data());
  for(int ch=0; ch<3; ch++){
    fprintf(fyout," %s & ", chname[ch].Data());
    fprintf(fyout," \$ %i \$ &",data[ch]);
    fprintf(fyout," \$ %8.1f \\pm %6.1f \$ &", 
    	    (signalSF[ch] + DY[ch] + Stop[ch] + VV[ch] + Non_WZ_DD[ch]),
    	    (sqrt(err_stat_signalSF[ch]**2 + err_DY[ch]**2 + err_stat_Stop[ch]**2 + err_stat_VV[ch]**2 + err_Non_WZ_DD[ch]**2)));
    fprintf(fyout," \$ %8.1f \\pm %6.1f \$ &", signalSF[ch],err_stat_signalSF[ch]);
    fprintf(fyout," \$ %8.1f \\pm %6.1f \$ &", DY[ch],err_DY[ch]);
    fprintf(fyout," \$ %8.1f \\pm %6.1f \$ &", Stop[ch],err_stat_Stop[ch]);
    fprintf(fyout," \$ %8.1f \\pm %6.1f \$ &", VV[ch],err_stat_VV[ch]);
    fprintf(fyout," \$ %8.1f \\pm %6.1f \$  ", Non_WZ_DD[ch],err_Non_WZ_DD[ch]);
    fprintf(fyout,"\\\\\\hline\n");
  }// for(ch) TeX

  fprintf(fyout,"\\end\{tabular\}\n");
  fprintf(fyout,"\}\n");
  fclose(fyout);


  TString texsysfileXsec=dirtexname + "Syst_Xsec" + cutname + ".tex";

  FILE* sysout_Xsec = fopen(texsysfileXsec, "w");

  fprintf(sysout_Xsec,"\\begin\{tabular\}\{|l|c|c|c|\}\n");
  fprintf(sysout_Xsec,"\\hline\n");
  fprintf(sysout_Xsec,"Source   \&  \\mmpm [pb] \&  \\eepm [pb] \&  \\empm [pb]  \\\\ \\hline \n"); 
  fprintf(sysout_Xsec,"\\hline\n");
  fprintf(sysout_Xsec,"Trigger efficiencies \& %.2f \& %.2f \& %.2f \\\\ \n", err_syst_xsec_cont[0][0],err_syst_xsec_cont[1][0],err_syst_xsec_cont[2][0]);
  fprintf(sysout_Xsec,"Lepton efficiencies \& %.2f \& %.2f \& %.2f \\\\ \n", err_syst_xsec_cont[0][1],err_syst_xsec_cont[1][1],err_syst_xsec_cont[2][1]);
  fprintf(sysout_Xsec,"Lepton energy scale \& %.2f \& %.2f \& %.2f \\\\ \n", err_syst_xsec_cont[0][2],err_syst_xsec_cont[1][2],err_syst_xsec_cont[2][2]);
  fprintf(sysout_Xsec,"Jet energy scale  \& %.2f \& %.2f \& %.2f \\\\ \n", err_syst_xsec_cont[0][3],err_syst_xsec_cont[1][3],err_syst_xsec_cont[2][3]);
  fprintf(sysout_Xsec,"Jet energy resolution  \& %.2f \& %.2f \& %.2f \\\\ \n", err_syst_xsec_cont[0][4],err_syst_xsec_cont[1][4],err_syst_xsec_cont[2][4]);
  fprintf(sysout_Xsec,"b-tagging \& %.2f \& %.2f \& %.2f \\\\ \n", err_syst_xsec_cont[0][5],err_syst_xsec_cont[1][5],err_syst_xsec_cont[2][5]);
  fprintf(sysout_Xsec,"Pileup \& %.2f \& %.2f \& %.2f \\\\ \n", err_syst_xsec_cont[0][6],err_syst_xsec_cont[1][6],err_syst_xsec_cont[2][6]);
  fprintf(sysout_Xsec,"Scale of QCD (\$\\mu\$)\& %.2f \& %.2f \& %.2f \\\\ \n", err_syst_xsec_cont[0][7],err_syst_xsec_cont[1][7],err_syst_xsec_cont[2][7]);
  fprintf(sysout_Xsec,"Matching partons to showers \& %.2f \& %.2f \& %.2f \\\\ \n", err_syst_xsec_cont[0][8],err_syst_xsec_cont[1][8],err_syst_xsec_cont[2][8]);
  fprintf(sysout_Xsec,"Experimental Xsec \& %.2f \& %.2f \& %.2f \\\\ \n",err_syst_xsec_cont[0][9],err_syst_xsec_cont[1][9],err_syst_xsec_cont[2][9]);
  fprintf(sysout_Xsec,"Drell-Yan DD \& %.2f \& %.2f \& %.2f \\\\ \n",err_syst_xsec_cont[0][10],err_syst_xsec_cont[1][10],err_syst_xsec_cont[2][10]);
  fprintf(sysout_Xsec,"Non-W/Z \& %.2f \& %.2f \& %.2f \\\\ \n",err_syst_xsec_cont[0][11],err_syst_xsec_cont[1][11],err_syst_xsec_cont[2][11]);
  fprintf(sysout_Xsec,"tW \& %.2f \& %.2f \& %.2f \\\\ \n",err_syst_xsec_cont[0][13],err_syst_xsec_cont[1][13],err_syst_xsec_cont[2][13]);
  fprintf(sysout_Xsec,"VV \& %.2f \& %.2f \& %.2f \\\\ \n",err_syst_xsec_cont[0][14],err_syst_xsec_cont[1][14],err_syst_xsec_cont[2][14]);
  
  //fprintf(sysout_Xsec,"Hadronization \& %.2f \& %.2f \& %.2f \\\\ \n",err_syst_xsec_cont[0][12],err_syst_xsec_cont[1][12],err_syst_xsec_cont[2][12]);
  fprintf(sysout_Xsec,"\\hline\n");
  fprintf(sysout_Xsec,"Total Systematic Uncertainty \& %.2f \& %.2f \& %.2f \\\\ \n",err_syst_xsec_cont[0][15],err_syst_xsec_cont[1][15],err_syst_xsec_cont[2][15]);
  fprintf(sysout_Xsec,"\\hline\n");
  fprintf(sysout_Xsec,"\\end\{tabular\}\n");

  fclose(sysout_Xsec);


  TString texsysfileXsec_per=dirtexname + "Syst_Xsec_per" + cutname + ".tex";

  FILE* sysout_Xsec_per = fopen(texsysfileXsec_per, "w");

  fprintf(sysout_Xsec_per,"\\begin\{tabular\}\{|l|c|c|c|\}\n");
  fprintf(sysout_Xsec_per,"\\hline\n");
  fprintf(sysout_Xsec_per,"Source   \&  \\mmpm [\\%] \&  \\eepm [\\%] \&  \\empm [\\%]  \\\\ \\hline \n"); 
  fprintf(sysout_Xsec_per,"\\hline\n");
  fprintf(sysout_Xsec_per,"Trigger efficiencies \& %.2f \& %.2f \& %.2f \\\\ \n", (100.0/xsec[0])*err_syst_xsec_cont[0][0],(100.0/xsec[1])*err_syst_xsec_cont[1][0],(100.0/xsec[2])*err_syst_xsec_cont[2][0]);
  fprintf(sysout_Xsec_per,"Lepton efficiencies \& %.2f \& %.2f \& %.2f \\\\ \n", (100.0/xsec[0])*err_syst_xsec_cont[0][1],(100.0/xsec[1])*err_syst_xsec_cont[1][1],(100.0/xsec[2])*err_syst_xsec_cont[2][1]);
  fprintf(sysout_Xsec_per,"Lepton energy scale \& %.2f \& %.2f \& %.2f \\\\ \n", (100.0/xsec[0])*err_syst_xsec_cont[0][2],(100.0/xsec[1])*err_syst_xsec_cont[1][2],(100.0/xsec[2])*err_syst_xsec_cont[2][2]);
  fprintf(sysout_Xsec_per,"Jet energy scale  \& %.2f \& %.2f \& %.2f \\\\ \n", (100.0/xsec[0])*err_syst_xsec_cont[0][3],(100.0/xsec[1])*err_syst_xsec_cont[1][3],(100.0/xsec[2])*err_syst_xsec_cont[2][3]);
  fprintf(sysout_Xsec_per,"Jet energy resolution  \& %.2f \& %.2f \& %.2f \\\\ \n", (100.0/xsec[0])*err_syst_xsec_cont[0][4],(100.0/xsec[1])*err_syst_xsec_cont[1][4],(100.0/xsec[2])*err_syst_xsec_cont[2][4]);
  fprintf(sysout_Xsec_per,"b-tagging \& %.2f \& %.2f \& %.2f \\\\ \n", (100.0/xsec[0])*err_syst_xsec_cont[0][5],(100.0/xsec[1])*err_syst_xsec_cont[1][5],(100.0/xsec[2])*err_syst_xsec_cont[2][5]);
  fprintf(sysout_Xsec_per,"Pileup \& %.2f \& %.2f \& %.2f \\\\ \n", (100.0/xsec[0])*err_syst_xsec_cont[0][6],(100.0/xsec[1])*err_syst_xsec_cont[1][6],(100.0/xsec[2])*err_syst_xsec_cont[2][6]);
  fprintf(sysout_Xsec_per,"Scale of QCD (\$\\mu\$)\& %.2f \& %.2f \& %.2f \\\\ \n", (100.0/xsec[0])*err_syst_xsec_cont[0][7],(100.0/xsec[1])*err_syst_xsec_cont[1][7],(100.0/xsec[2])*err_syst_xsec_cont[2][7]);
  fprintf(sysout_Xsec_per,"Matching partons to showers \& %.2f \& %.2f \& %.2f \\\\ \n", (100.0/xsec[0])*err_syst_xsec_cont[0][8],(100.0/xsec[1])*err_syst_xsec_cont[1][8],(100.0/xsec[2])*err_syst_xsec_cont[2][8]);
  fprintf(sysout_Xsec_per,"Experimental Xsec \& %.2f \& %.2f \& %.2f \\\\ \n",(100.0/xsec[0])*err_syst_xsec_cont[0][9],(100.0/xsec[1])*err_syst_xsec_cont[1][9],(100.0/xsec[2])*err_syst_xsec_cont[2][9]);
  fprintf(sysout_Xsec_per,"Drell-Yan DD \& %.2f \& %.2f \& %.2f \\\\ \n",(100.0/xsec[0])*err_syst_xsec_cont[0][10],(100.0/xsec[1])*err_syst_xsec_cont[1][10],(100.0/xsec[2])*err_syst_xsec_cont[2][10]);
  fprintf(sysout_Xsec_per,"Non-W/Z \& %.2f \& %.2f \& %.2f \\\\ \n",(100.0/xsec[0])*err_syst_xsec_cont[0][11],(100.0/xsec[1])*err_syst_xsec_cont[1][11],(100.0/xsec[2])*err_syst_xsec_cont[2][11]);
  fprintf(sysout_Xsec_per,"tW \& %.2f \& %.2f \& %.2f \\\\ \n",(100.0/xsec[0])*err_syst_xsec_cont[0][13],(100.0/xsec[1])*err_syst_xsec_cont[1][13],(100.0/xsec[2])*err_syst_xsec_cont[2][13]);
  fprintf(sysout_Xsec_per,"VV \& %.2f \& %.2f \& %.2f \\\\ \n",(100.0/xsec[0])*err_syst_xsec_cont[0][14],(100.0/xsec[1])*err_syst_xsec_cont[1][14],(100.0/xsec[2])*err_syst_xsec_cont[2][14]);
  
  //fprintf(sysout_Xsec_per,"Hadronization \& %.2f \& %.2f \& %.2f \\\\ \n",(100.0/xsec[0])*err_syst_xsec_cont[0][12],(100.0/xsec[1])*err_syst_xsec_cont[1][12],(100.0/xsec[2])*err_syst_xsec_cont[2][12]);
  fprintf(sysout_Xsec_per,"\\hline\n");
  fprintf(sysout_Xsec_per,"Total Systematic Uncertainty \& %.2f \& %.2f \& %.2f \\\\ \n",(100.0/xsec[0])*err_syst_xsec_cont[0][15],(100.0/xsec[1])*err_syst_xsec_cont[1][15],(100.0/xsec[2])*err_syst_xsec_cont[2][15]);
  fprintf(sysout_Xsec_per,"\\hline\n");
  fprintf(sysout_Xsec_per,"\\end\{tabular\}\n");

  fclose(sysout_Xsec_per);


  TString texsysfileTTbar=dirtexname + "Syst_TTbar" + cutname + ".tex";

  FILE* sysout_TTbar = fopen(texsysfileTTbar, "w");

  fprintf(sysout_TTbar,"\\begin\{tabular\}\{|l|c|c|c|\}\n");
  fprintf(sysout_TTbar,"\\hline\n");
  fprintf(sysout_TTbar,"Source   \&  \\mmpm [\\%] \&  \\eepm [\\%] \&  \\empm [\\%]  \\\\ \\hline \n"); 
  fprintf(sysout_TTbar,"\\hline\n");
  fprintf(sysout_TTbar,"Trigger efficiencies \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*sig_Unc[0][0],100.*sig_Unc[1][0],100.*sig_Unc[2][0]);
  fprintf(sysout_TTbar,"Lepton efficiencies \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*sig_Unc[0][1],100.*sig_Unc[1][1],100.*sig_Unc[2][1]);
  fprintf(sysout_TTbar,"Lepton energy scale \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*sig_Unc[0][2],100.*sig_Unc[1][2],100.*sig_Unc[2][2]);
  fprintf(sysout_TTbar,"Jet energy scale  \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*sig_Unc[0][3],100.*sig_Unc[1][3],100.*sig_Unc[2][3]);
  fprintf(sysout_TTbar,"Jet energy resolution  \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*sig_Unc[0][4],100.*sig_Unc[1][4],100.*sig_Unc[2][4]);
  fprintf(sysout_TTbar,"b-tagging \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*sig_Unc[0][5],100.*sig_Unc[1][5],100.*sig_Unc[2][5]);
  fprintf(sysout_TTbar,"Pileup \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*sig_Unc[0][6],100.*sig_Unc[1][6],100.*sig_Unc[2][6]);
  fprintf(sysout_TTbar,"Scale of QCD (\$\\mu\$)\& %.2f \& %.2f \& %.2f \\\\ \n", 100.*sig_Unc[0][7],100.*sig_Unc[1][7],100.*sig_Unc[2][7]);
  fprintf(sysout_TTbar,"Matching partons to showers \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*sig_Unc[0][8],100.*sig_Unc[1][8],100.*sig_Unc[2][8]);
  //fprintf(sysout_TTbar,"Hadronization \& %.2f \& %.2f \& %.2f \\\\ \n",100.*sig_Unc[0][12],100.*sig_Unc[1][12],100.*sig_Unc[2][12]);
  fprintf(sysout_TTbar,"\\hline\n");
  fprintf(sysout_TTbar,"\\end\{tabular\}\n");

  fclose(sysout_TTbar);

  TString texsysfileTW=dirtexname + "Syst_TW" + cutname + ".tex";

  FILE* sysout_TW = fopen(texsysfileTW, "w");

  fprintf(sysout_TW,"\\begin\{tabular\}\{|l|c|c|c|\}\n");
  fprintf(sysout_TW,"\\hline\n");
  fprintf(sysout_TW,"Source   \&  \\mmpm [\\%] \&  \\eepm [\\%] \&  \\empm [\\%]  \\\\ \\hline \n"); 
  fprintf(sysout_TW,"\\hline\n");
  //fprintf(sysout_TW,"Trigger efficiencies \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*TW_Unc[0][0],100.*TW_Unc[1][0],100.*TW_Unc[2][0]);
  //fprintf(sysout_TW,"Lepton efficiencies \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*TW_Unc[0][1],100.*TW_Unc[1][1],100.*TW_Unc[2][1]);
  fprintf(sysout_TW,"Lepton energy scale \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*TW_Unc[0][2],100.*TW_Unc[1][2],100.*TW_Unc[2][2]);
  fprintf(sysout_TW,"Jet energy scale  \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*TW_Unc[0][3],100.*TW_Unc[1][3],100.*TW_Unc[2][3]);
  fprintf(sysout_TW,"Jet energy resolution  \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*TW_Unc[0][4],100.*TW_Unc[1][4],100.*TW_Unc[2][4]);
  fprintf(sysout_TW,"b-tagging \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*TW_Unc[0][5],100.*TW_Unc[1][5],100.*TW_Unc[2][5]);
  fprintf(sysout_TW,"Pileup \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*TW_Unc[0][6],100.*TW_Unc[1][6],100.*TW_Unc[2][6]);
  //fprintf(sysout_TW,"Scale of QCD (\$\\mu\$)\& %.2f \& %.2f \& %.2f \\\\ \n", 100.*TW_Unc[0][7],100.*TW_Unc[1][7],100.*TW_Unc[2][7]);
  //fprintf(sysout_TW,"Matching partons to showers \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*TW_Unc[0][8],100.*TW_Unc[1][8],100.*TW_Unc[2][8]);
  //fprintf(sysout_TW,"Hadronization \& %.2f \& %.2f \& %.2f \\\\ \n",100.*TW_Unc[0][12],100.*TW_Unc[1][12],100.*TW_Unc[2][12]);
  fprintf(sysout_TW,"\\hline\n");
  fprintf(sysout_TW,"\\end\{tabular\}\n");

  fclose(sysout_TW);

  TString texsysfileVV=dirtexname + "Syst_VV" + cutname + ".tex";
  FILE* sysout_VV = fopen(texsysfileVV, "w");

  fprintf(sysout_VV,"\\begin\{tabular\}\{|l|c|c|c|\}\n");
  fprintf(sysout_VV,"\\hline\n");
  fprintf(sysout_VV,"Source   \&  \\mmpm [\\%] \&  \\eepm [\\%] \&  \\empm [\\%]  \\\\ \\hline \n"); 
  fprintf(sysout_VV,"\\hline\n");
  //fprintf(sysout_VV,"Trigger efficiencies \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*VV_Unc[0][0],100.*VV_Unc[1][0],100.*VV_Unc[2][0]);
  //fprintf(sysout_VV,"Lepton efficiencies \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*VV_Unc[0][1],100.*VV_Unc[1][1],100.*VV_Unc[2][1]);
  fprintf(sysout_VV,"Lepton energy scale \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*VV_Unc[0][2],100.*VV_Unc[1][2],100.*VV_Unc[2][2]);
  fprintf(sysout_VV,"Jet energy scale  \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*VV_Unc[0][3],100.*VV_Unc[1][3],100.*VV_Unc[2][3]);
  fprintf(sysout_VV,"Jet energy resolution  \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*VV_Unc[0][4],100.*VV_Unc[1][4],100.*VV_Unc[2][4]);
  fprintf(sysout_VV,"b-tagging \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*VV_Unc[0][5],100.*VV_Unc[1][5],100.*VV_Unc[2][5]);
  fprintf(sysout_VV,"Pileup \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*VV_Unc[0][6],100.*VV_Unc[1][6],100.*VV_Unc[2][6]);
  //fprintf(sysout_VV,"Scale of QCD (\$\\mu\$)\& %.2f \& %.2f \& %.2f \\\\ \n", 100.*VV_Unc[0][7],100.*VV_Unc[1][7],100.*VV_Unc[2][7]);
  //fprintf(sysout_VV,"Matching partons to showers \& %.2f \& %.2f \& %.2f \\\\ \n", 100.*VV_Unc[0][8],100.*VV_Unc[1][8],100.*VV_Unc[2][8]);
  //fprintf(sysout_VV,"Hadronization \& %.2f \& %.2f \& %.2f \\\\ \n",100.*VV_Unc[0][12],100.*VV_Unc[1][12],100.*VV_Unc[2][12]);
  fprintf(sysout_VV,"\\hline\n");
  fprintf(sysout_VV,"\\end\{tabular\}\n");

  fclose(sysout_VV);


  //--------------------------------------------------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------------------------------------------------

  TString comtexsysfile=dirtexname + "Syst" + cutname + "_Complete.tex";

  FILE* comsysout = fopen(comtexsysfile, "w");

  fprintf(comsysout,"\\footnotesize\{\n");
  fprintf(comsysout,"\\begin\{tabular\}\{|l|c|c|c|c||c|c|c||c|c|c|\}\n");
  fprintf(comsysout,"\\hline\n");
  fprintf(comsysout,"Source   \&  \& \\mumu \&  \\ee \&  \\mue \& \\mumu \&  \\ee \&  \\mue \& \\mumu \&  \\ee \&  \\mue  \\\\ \\hline \n"); 
  fprintf(comsysout,"\\hline\n");

  /////////////////////////////////////////
  // Trigger Efficiency [0]
  /////////////////////////////////////////

  fprintf(comsysout,"\\multirow\{3\}\{*\}\{Trigger efficiencies\} \& Up \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \& \\multirow\{3\}\{*\}\{%.2f\} \& \\multirow\{3\}\{*\}\{%.2f\} \& \\multirow\{3\}\{*\}\{%.2f\} \\\\ \n", TTbar_MadSpin_Tr_Up[0][cut], TTbar_MadSpin_Tr_Up[1][cut], TTbar_MadSpin_Tr_Up[2][cut], 100.*fabs (TTbar_MadSpin_Tr_Up[0][cut] - TTbar_MadSpin_Tr_Central[0][cut])/TTbar_MadSpin_Tr_Central[0][cut], 100.*fabs (TTbar_MadSpin_Tr_Up[1][cut] - TTbar_MadSpin_Tr_Central[1][cut])/TTbar_MadSpin_Tr_Central[1][cut], 100.*fabs (TTbar_MadSpin_Tr_Up[2][cut] - TTbar_MadSpin_Tr_Central[2][cut])/TTbar_MadSpin_Tr_Central[2][cut],100.*sig_Unc[0][0],100.*sig_Unc[1][0],100.*sig_Unc[2][0]);
  
  fprintf(comsysout,"\& \\color{red}{Central} \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \&  \& \& \\\\ \n", TTbar_MadSpin_Tr_Central[0][cut], TTbar_MadSpin_Tr_Central[1][cut], TTbar_MadSpin_Tr_Central[2][cut], 100.*fabs (TTbar_MadSpin_Tr_Central[0][cut] - TTbar_MadSpin_Tr_Central[0][cut])/TTbar_MadSpin_Tr_Central[0][cut], 100.*fabs (TTbar_MadSpin_Tr_Central[1][cut] - TTbar_MadSpin_Tr_Central[1][cut])/TTbar_MadSpin_Tr_Central[1][cut], 100.*fabs (TTbar_MadSpin_Tr_Central[2][cut] - TTbar_MadSpin_Tr_Central[2][cut])/TTbar_MadSpin_Tr_Central[2][cut]);

  fprintf(comsysout," \& Down \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \&  \&  \&  \\\\ \n", TTbar_MadSpin_Tr_Down[0][cut], TTbar_MadSpin_Tr_Down[1][cut], TTbar_MadSpin_Tr_Down[2][cut], 100.*fabs (TTbar_MadSpin_Tr_Down[0][cut] - TTbar_MadSpin_Tr_Central[0][cut])/TTbar_MadSpin_Tr_Central[0][cut], 100.*fabs (TTbar_MadSpin_Tr_Down[1][cut] - TTbar_MadSpin_Tr_Central[1][cut])/TTbar_MadSpin_Tr_Central[1][cut], 100.*fabs (TTbar_MadSpin_Tr_Down[2][cut] - TTbar_MadSpin_Tr_Central[2][cut])/TTbar_MadSpin_Tr_Central[2][cut]);

  fprintf(comsysout,"\\hline\n");

  /////////////////////////////////////////
  // Lepton Efficiency [1]
  /////////////////////////////////////////

  fprintf(comsysout,"\\multirow\{3\}\{*\}\{Lepton efficiencies\} \& Up \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \& \\multirow\{3\}\{*\}\{%.2f\} \& \\multirow\{3\}\{*\}\{%.2f\} \& \\multirow\{3\}\{*\}\{%.2f\} \\\\ \n", TTbar_MadSpin_IDISO_Up[0][cut], TTbar_MadSpin_IDISO_Up[1][cut], TTbar_MadSpin_IDISO_Up[2][cut], 100.*fabs (TTbar_MadSpin_IDISO_Up[0][cut] - TTbar_MadSpin_IDISO_Central[0][cut])/TTbar_MadSpin_IDISO_Central[0][cut], 100.*fabs (TTbar_MadSpin_IDISO_Up[1][cut] - TTbar_MadSpin_IDISO_Central[1][cut])/TTbar_MadSpin_IDISO_Central[1][cut], 100.*fabs (TTbar_MadSpin_IDISO_Up[2][cut] - TTbar_MadSpin_IDISO_Central[2][cut])/TTbar_MadSpin_IDISO_Central[2][cut],100.*sig_Unc[0][1],100.*sig_Unc[1][1],100.*sig_Unc[2][1]);
  
  fprintf(comsysout,"\& \\color{red}{Central} \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \&  \& \& \\\\ \n", TTbar_MadSpin_IDISO_Central[0][cut], TTbar_MadSpin_IDISO_Central[1][cut], TTbar_MadSpin_IDISO_Central[2][cut], 100.*fabs (TTbar_MadSpin_IDISO_Central[0][cut] - TTbar_MadSpin_IDISO_Central[0][cut])/TTbar_MadSpin_IDISO_Central[0][cut], 100.*fabs (TTbar_MadSpin_IDISO_Central[1][cut] - TTbar_MadSpin_IDISO_Central[1][cut])/TTbar_MadSpin_IDISO_Central[1][cut], 100.*fabs (TTbar_MadSpin_IDISO_Central[2][cut] - TTbar_MadSpin_IDISO_Central[2][cut])/TTbar_MadSpin_IDISO_Central[2][cut]);

  fprintf(comsysout," \& Down \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \&  \&  \&  \\\\ \n", TTbar_MadSpin_IDISO_Down[0][cut], TTbar_MadSpin_IDISO_Down[1][cut], TTbar_MadSpin_IDISO_Down[2][cut], 100.*fabs (TTbar_MadSpin_IDISO_Down[0][cut] - TTbar_MadSpin_IDISO_Central[0][cut])/TTbar_MadSpin_IDISO_Central[0][cut], 100.*fabs (TTbar_MadSpin_IDISO_Down[1][cut] - TTbar_MadSpin_IDISO_Central[1][cut])/TTbar_MadSpin_IDISO_Central[1][cut], 100.*fabs (TTbar_MadSpin_IDISO_Down[2][cut] - TTbar_MadSpin_IDISO_Central[2][cut])/TTbar_MadSpin_IDISO_Central[2][cut]);

  fprintf(comsysout,"\\hline\n");
  
  /////////////////////////////////////////
  // Lepton Energy Scale [2]
  /////////////////////////////////////////

  fprintf(comsysout,"\\multirow\{3\}\{*\}\{Lepton energy scale\} \& Up \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \& \\multirow\{3\}\{*\}\{%.2f\} \& \\multirow\{3\}\{*\}\{%.2f\} \& \\multirow\{3\}\{*\}\{%.2f\} \\\\ \n", TTbar_MadSpin_LES_Up[0][cut], TTbar_MadSpin_LES_Up[1][cut], TTbar_MadSpin_LES_Up[2][cut], 100.*fabs (TTbar_MadSpin_LES_Up[0][cut] - TTbar_MadSpin_Central[0][cut])/TTbar_MadSpin_Central[0][cut], 100.*fabs (TTbar_MadSpin_LES_Up[1][cut] - TTbar_MadSpin_Central[1][cut])/TTbar_MadSpin_Central[1][cut], 100.*fabs (TTbar_MadSpin_LES_Up[2][cut] - TTbar_MadSpin_Central[2][cut])/TTbar_MadSpin_Central[2][cut],100.*sig_Unc[0][2],100.*sig_Unc[1][2],100.*sig_Unc[2][2]);
  
  fprintf(comsysout,"\& Central \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \&  \& \& \\\\ \n", TTbar_MadSpin_Central[0][cut], TTbar_MadSpin_Central[1][cut], TTbar_MadSpin_Central[2][cut], 100.*fabs (TTbar_MadSpin_Central[0][cut] - TTbar_MadSpin_Central[0][cut])/TTbar_MadSpin_Central[0][cut], 100.*fabs (TTbar_MadSpin_Central[1][cut] - TTbar_MadSpin_Central[1][cut])/TTbar_MadSpin_Central[1][cut], 100.*fabs (TTbar_MadSpin_Central[2][cut] - TTbar_MadSpin_Central[2][cut])/TTbar_MadSpin_Central[2][cut]);

  fprintf(comsysout," \& Down \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \&  \&  \&  \\\\ \n", TTbar_MadSpin_LES_Down[0][cut], TTbar_MadSpin_LES_Down[1][cut], TTbar_MadSpin_LES_Down[2][cut], 100.*fabs (TTbar_MadSpin_LES_Down[0][cut] - TTbar_MadSpin_Central[0][cut])/TTbar_MadSpin_Central[0][cut], 100.*fabs (TTbar_MadSpin_LES_Down[1][cut] - TTbar_MadSpin_Central[1][cut])/TTbar_MadSpin_Central[1][cut], 100.*fabs (TTbar_MadSpin_LES_Down[2][cut] - TTbar_MadSpin_Central[2][cut])/TTbar_MadSpin_Central[2][cut]);

  fprintf(comsysout,"\\hline\n");
  
  /////////////////////////////////////////
  // Jet Energy Scale [3]
  /////////////////////////////////////////

  fprintf(comsysout,"\\multirow\{3\}\{*\}\{Jet energy scale\} \& Up \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \& \\multirow\{3\}\{*\}\{%.2f\} \& \\multirow\{3\}\{*\}\{%.2f\} \& \\multirow\{3\}\{*\}\{%.2f\} \\\\ \n", TTbar_MadSpin_JES_Up[0][cut], TTbar_MadSpin_JES_Up[1][cut], TTbar_MadSpin_JES_Up[2][cut], 100.*fabs (TTbar_MadSpin_JES_Up[0][cut] - TTbar_MadSpin_Central[0][cut])/TTbar_MadSpin_Central[0][cut], 100.*fabs (TTbar_MadSpin_JES_Up[1][cut] - TTbar_MadSpin_Central[1][cut])/TTbar_MadSpin_Central[1][cut], 100.*fabs (TTbar_MadSpin_JES_Up[2][cut] - TTbar_MadSpin_Central[2][cut])/TTbar_MadSpin_Central[2][cut],100.*sig_Unc[0][3],100.*sig_Unc[1][3],100.*sig_Unc[2][3]);
  
  fprintf(comsysout,"\& Central \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \&  \& \& \\\\ \n", TTbar_MadSpin_Central[0][cut], TTbar_MadSpin_Central[1][cut], TTbar_MadSpin_Central[2][cut], 100.*fabs (TTbar_MadSpin_Central[0][cut] - TTbar_MadSpin_Central[0][cut])/TTbar_MadSpin_Central[0][cut], 100.*fabs (TTbar_MadSpin_Central[1][cut] - TTbar_MadSpin_Central[1][cut])/TTbar_MadSpin_Central[1][cut], 100.*fabs (TTbar_MadSpin_Central[2][cut] - TTbar_MadSpin_Central[2][cut])/TTbar_MadSpin_Central[2][cut]);

  fprintf(comsysout," \& Down \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \&  \&  \&  \\\\ \n", TTbar_MadSpin_JES_Down[0][cut], TTbar_MadSpin_JES_Down[1][cut], TTbar_MadSpin_JES_Down[2][cut], 100.*fabs (TTbar_MadSpin_JES_Down[0][cut] - TTbar_MadSpin_Central[0][cut])/TTbar_MadSpin_Central[0][cut], 100.*fabs (TTbar_MadSpin_JES_Down[1][cut] - TTbar_MadSpin_Central[1][cut])/TTbar_MadSpin_Central[1][cut], 100.*fabs (TTbar_MadSpin_JES_Down[2][cut] - TTbar_MadSpin_Central[2][cut])/TTbar_MadSpin_Central[2][cut]);

  fprintf(comsysout,"\\hline\n");

  /////////////////////////////////////////
  // Jet Energy Resolution [4]
  /////////////////////////////////////////

  fprintf(comsysout,"\\multirow\{3\}\{*\}\{Jet energy resolution\} \& Up \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \& \\multirow\{3\}\{*\}\{%.2f\} \& \\multirow\{3\}\{*\}\{%.2f\} \& \\multirow\{3\}\{*\}\{%.2f\} \\\\ \n", TTbar_MadSpin_JER_Up[0][cut], TTbar_MadSpin_JER_Up[1][cut], TTbar_MadSpin_JER_Up[2][cut], 100.*fabs (TTbar_MadSpin_JER_Up[0][cut] - TTbar_MadSpin_JER_Nom[0][cut])/TTbar_MadSpin_JER_Nom[0][cut], 100.*fabs (TTbar_MadSpin_JER_Up[1][cut] - TTbar_MadSpin_JER_Nom[1][cut])/TTbar_MadSpin_JER_Nom[1][cut], 100.*fabs (TTbar_MadSpin_JER_Up[2][cut] - TTbar_MadSpin_JER_Nom[2][cut])/TTbar_MadSpin_JER_Nom[2][cut],100.*sig_Unc[0][4],100.*sig_Unc[1][4],100.*sig_Unc[2][4]);
  
  fprintf(comsysout,"\& \\color{red}{Central} \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \&  \& \& \\\\ \n", TTbar_MadSpin_JER_Nom[0][cut], TTbar_MadSpin_JER_Nom[1][cut], TTbar_MadSpin_JER_Nom[2][cut], 100.*fabs (TTbar_MadSpin_JER_Nom[0][cut] - TTbar_MadSpin_JER_Nom[0][cut])/TTbar_MadSpin_JER_Nom[0][cut], 100.*fabs (TTbar_MadSpin_JER_Nom[1][cut] - TTbar_MadSpin_JER_Nom[1][cut])/TTbar_MadSpin_JER_Nom[1][cut], 100.*fabs (TTbar_MadSpin_JER_Nom[2][cut] - TTbar_MadSpin_JER_Nom[2][cut])/TTbar_MadSpin_JER_Nom[2][cut]);

  fprintf(comsysout," \& Down \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \&  \&  \&  \\\\ \n", TTbar_MadSpin_JER_Down[0][cut], TTbar_MadSpin_JER_Down[1][cut], TTbar_MadSpin_JER_Down[2][cut], 100.*fabs (TTbar_MadSpin_JER_Down[0][cut] - TTbar_MadSpin_JER_Nom[0][cut])/TTbar_MadSpin_JER_Nom[0][cut], 100.*fabs (TTbar_MadSpin_JER_Down[1][cut] - TTbar_MadSpin_JER_Nom[1][cut])/TTbar_MadSpin_JER_Nom[1][cut], 100.*fabs (TTbar_MadSpin_JER_Down[2][cut] - TTbar_MadSpin_JER_Nom[2][cut])/TTbar_MadSpin_JER_Nom[2][cut]);

  fprintf(comsysout,"\\hline\n");

  /////////////////////////////////////////
  // b-tagging [5]
  /////////////////////////////////////////

  fprintf(comsysout,"\\multirow\{3\}\{*\}\{b-jet tagging\} \& Up \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \& \\multirow\{3\}\{*\}\{%.2f\} \& \\multirow\{3\}\{*\}\{%.2f\} \& \\multirow\{3\}\{*\}\{%.2f\} \\\\ \n", TTbar_MadSpin_btag_Up[0][cut], TTbar_MadSpin_btag_Up[1][cut], TTbar_MadSpin_btag_Up[2][cut], 100.*fabs (TTbar_MadSpin_btag_Up[0][cut] - TTbar_MadSpin_Central[0][cut])/TTbar_MadSpin_Central[0][cut], 100.*fabs (TTbar_MadSpin_btag_Up[1][cut] - TTbar_MadSpin_Central[1][cut])/TTbar_MadSpin_Central[1][cut], 100.*fabs (TTbar_MadSpin_btag_Up[2][cut] - TTbar_MadSpin_Central[2][cut])/TTbar_MadSpin_Central[2][cut],100.*sig_Unc[0][5],100.*sig_Unc[1][5],100.*sig_Unc[2][5]);
  
  fprintf(comsysout,"\& Central \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \&  \& \& \\\\ \n", TTbar_MadSpin_Central[0][cut], TTbar_MadSpin_Central[1][cut], TTbar_MadSpin_Central[2][cut], 100.*fabs (TTbar_MadSpin_Central[0][cut] - TTbar_MadSpin_Central[0][cut])/TTbar_MadSpin_Central[0][cut], 100.*fabs (TTbar_MadSpin_Central[1][cut] - TTbar_MadSpin_Central[1][cut])/TTbar_MadSpin_Central[1][cut], 100.*fabs (TTbar_MadSpin_Central[2][cut] - TTbar_MadSpin_Central[2][cut])/TTbar_MadSpin_Central[2][cut]);

  fprintf(comsysout," \& Down \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \&  \&  \&  \\\\ \n", TTbar_MadSpin_btag_Down[0][cut], TTbar_MadSpin_btag_Down[1][cut], TTbar_MadSpin_btag_Down[2][cut], 100.*fabs (TTbar_MadSpin_btag_Down[0][cut] - TTbar_MadSpin_Central[0][cut])/TTbar_MadSpin_Central[0][cut], 100.*fabs (TTbar_MadSpin_btag_Down[1][cut] - TTbar_MadSpin_Central[1][cut])/TTbar_MadSpin_Central[1][cut], 100.*fabs (TTbar_MadSpin_btag_Down[2][cut] - TTbar_MadSpin_Central[2][cut])/TTbar_MadSpin_Central[2][cut]);


  fprintf(comsysout,"\\hline\n");

  /////////////////////////////////////////
  // PileUp [6]
  /////////////////////////////////////////


  fprintf(comsysout,"\\multirow\{3\}\{*\}\{Pileup\} \& Up \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \& \\multirow\{3\}\{*\}\{%.2f\} \& \\multirow\{3\}\{*\}\{%.2f\} \& \\multirow\{3\}\{*\}\{%.2f\} \\\\ \n", TTbar_MadSpin_PU_Up[0][cut], TTbar_MadSpin_PU_Up[1][cut], TTbar_MadSpin_PU_Up[2][cut], 100.*fabs (TTbar_MadSpin_PU_Up[0][cut] - TTbar_MadSpin_Central[0][cut])/TTbar_MadSpin_Central[0][cut], 100.*fabs (TTbar_MadSpin_PU_Up[1][cut] - TTbar_MadSpin_Central[1][cut])/TTbar_MadSpin_Central[1][cut], 100.*fabs (TTbar_MadSpin_PU_Up[2][cut] - TTbar_MadSpin_Central[2][cut])/TTbar_MadSpin_Central[2][cut],100.*sig_Unc[0][6],100.*sig_Unc[1][6],100.*sig_Unc[2][6]);
  
  fprintf(comsysout,"\& Central \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \&  \& \& \\\\ \n", TTbar_MadSpin_Central[0][cut], TTbar_MadSpin_Central[1][cut], TTbar_MadSpin_Central[2][cut], 100.*fabs (TTbar_MadSpin_Central[0][cut] - TTbar_MadSpin_Central[0][cut])/TTbar_MadSpin_Central[0][cut], 100.*fabs (TTbar_MadSpin_Central[1][cut] - TTbar_MadSpin_Central[1][cut])/TTbar_MadSpin_Central[1][cut], 100.*fabs (TTbar_MadSpin_Central[2][cut] - TTbar_MadSpin_Central[2][cut])/TTbar_MadSpin_Central[2][cut]);

  fprintf(comsysout," \& Down \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \&  \&  \&  \\\\ \n", TTbar_MadSpin_PU_Down[0][cut], TTbar_MadSpin_PU_Down[1][cut], TTbar_MadSpin_PU_Down[2][cut], 100.*fabs (TTbar_MadSpin_PU_Down[0][cut] - TTbar_MadSpin_Central[0][cut])/TTbar_MadSpin_Central[0][cut], 100.*fabs (TTbar_MadSpin_PU_Down[1][cut] - TTbar_MadSpin_Central[1][cut])/TTbar_MadSpin_Central[1][cut], 100.*fabs (TTbar_MadSpin_PU_Down[2][cut] - TTbar_MadSpin_Central[2][cut])/TTbar_MadSpin_Central[2][cut]);


  fprintf(comsysout,"\\hline\n");

  /////////////////////////////////////////
  // Scale [7]
  /////////////////////////////////////////


  fprintf(comsysout,"\\multirow\{3\}\{*\}\{Scale QCD\} \& Up \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \& \\multirow\{3\}\{*\}\{%.2f\} \& \\multirow\{3\}\{*\}\{%.2f\} \& \\multirow\{3\}\{*\}\{%.2f\} \\\\ \n", TTbar_MadSpin_Scale_Up[0][cut], TTbar_MadSpin_Scale_Up[1][cut], TTbar_MadSpin_Scale_Up[2][cut], 100.*fabs (TTbar_MadSpin_Scale_Up[0][cut] - TTbar_MadSpin_Central[0][cut])/TTbar_MadSpin_Central[0][cut], 100.*fabs (TTbar_MadSpin_Scale_Up[1][cut] - TTbar_MadSpin_Central[1][cut])/TTbar_MadSpin_Central[1][cut], 100.*fabs (TTbar_MadSpin_Scale_Up[2][cut] - TTbar_MadSpin_Central[2][cut])/TTbar_MadSpin_Central[2][cut],100.*sig_Unc[0][7],100.*sig_Unc[1][7],100.*sig_Unc[2][7]);
  
  fprintf(comsysout,"\& Central \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \&  \& \& \\\\ \n", TTbar_MadSpin_Central[0][cut], TTbar_MadSpin_Central[1][cut], TTbar_MadSpin_Central[2][cut], 100.*fabs (TTbar_MadSpin_Central[0][cut] - TTbar_MadSpin_Central[0][cut])/TTbar_MadSpin_Central[0][cut], 100.*fabs (TTbar_MadSpin_Central[1][cut] - TTbar_MadSpin_Central[1][cut])/TTbar_MadSpin_Central[1][cut], 100.*fabs (TTbar_MadSpin_Central[2][cut] - TTbar_MadSpin_Central[2][cut])/TTbar_MadSpin_Central[2][cut]);

  fprintf(comsysout," \& Down \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \&  \&  \&  \\\\ \n", TTbar_MadSpin_Scale_Down[0][cut], TTbar_MadSpin_Scale_Down[1][cut], TTbar_MadSpin_Scale_Down[2][cut], 100.*fabs (TTbar_MadSpin_Scale_Down[0][cut] - TTbar_MadSpin_Central[0][cut])/TTbar_MadSpin_Central[0][cut], 100.*fabs (TTbar_MadSpin_Scale_Down[1][cut] - TTbar_MadSpin_Central[1][cut])/TTbar_MadSpin_Central[1][cut], 100.*fabs (TTbar_MadSpin_Scale_Down[2][cut] - TTbar_MadSpin_Central[2][cut])/TTbar_MadSpin_Central[2][cut]);

  fprintf(comsysout,"\\hline\n");

  /////////////////////////////////////////
  // Matching [8]
  /////////////////////////////////////////


  fprintf(comsysout,"\\multirow\{3\}\{*\}\{Matching\} \& Up \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \& \\multirow\{3\}\{*\}\{%.2f\} \& \\multirow\{3\}\{*\}\{%.2f\} \& \\multirow\{3\}\{*\}\{%.2f\} \\\\ \n", TTbar_MadSpin_Matching_Up[0][cut], TTbar_MadSpin_Matching_Up[1][cut], TTbar_MadSpin_Matching_Up[2][cut], 100.*fabs (TTbar_MadSpin_Matching_Up[0][cut] - TTbar_MadSpin_Central[0][cut])/TTbar_MadSpin_Central[0][cut], 100.*fabs (TTbar_MadSpin_Matching_Up[1][cut] - TTbar_MadSpin_Central[1][cut])/TTbar_MadSpin_Central[1][cut], 100.*fabs (TTbar_MadSpin_Matching_Up[2][cut] - TTbar_MadSpin_Central[2][cut])/TTbar_MadSpin_Central[2][cut],100.*sig_Unc[0][8],100.*sig_Unc[1][8],100.*sig_Unc[2][8]);
  
  fprintf(comsysout,"\& Central \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \&  \& \& \\\\ \n", TTbar_MadSpin_Central[0][cut], TTbar_MadSpin_Central[1][cut], TTbar_MadSpin_Central[2][cut], 100.*fabs (TTbar_MadSpin_Central[0][cut] - TTbar_MadSpin_Central[0][cut])/TTbar_MadSpin_Central[0][cut], 100.*fabs (TTbar_MadSpin_Central[1][cut] - TTbar_MadSpin_Central[1][cut])/TTbar_MadSpin_Central[1][cut], 100.*fabs (TTbar_MadSpin_Central[2][cut] - TTbar_MadSpin_Central[2][cut])/TTbar_MadSpin_Central[2][cut],100.*sig_Unc[0][0],100.*sig_Unc[1][0],100.*sig_Unc[2][0]);

  fprintf(comsysout," \& Down \& %.1f \& %.1f \& %.1f \& %.2f \& %.2f \& %.2f  \&  \&  \&  \\\\ \n", TTbar_MadSpin_Matching_Down[0][cut], TTbar_MadSpin_Matching_Down[1][cut], TTbar_MadSpin_Matching_Down[2][cut], 100.*fabs (TTbar_MadSpin_Matching_Down[0][cut] - TTbar_MadSpin_Central[0][cut])/TTbar_MadSpin_Central[0][cut], 100.*fabs (TTbar_MadSpin_Matching_Down[1][cut] - TTbar_MadSpin_Central[1][cut])/TTbar_MadSpin_Central[1][cut], 100.*fabs (TTbar_MadSpin_Matching_Down[2][cut] - TTbar_MadSpin_Central[2][cut])/TTbar_MadSpin_Central[2][cut],100.*sig_Unc[0][0],100.*sig_Unc[1][0],100.*sig_Unc[2][0]);


  fprintf(comsysout,"\\hline\n");
  fprintf(comsysout,"\\end\{tabular\}\n");
  fprintf(comsysout,"\}\n");

  fclose(comsysout);
  
}
