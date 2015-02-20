#ifndef __CINT__

#include<string>
#include<iostream>
#include<sstream>
#include<stdio.h>
#include<stdlib.h>
#include<cmath>
#include<set>
#include<vector>

#endif

// Root
#include "TDirectory.h"
#include "TROOT.h"
#include "TObject.h"
#include "TCanvas.h"
#include "TStopwatch.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TFile.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include "TSystem.h"
#include "TTree.h"
//#include "TKey.h"
//#include "TPrint.h"
//#include <exception>
#include <sys/stat.h>

// TopCode
#include <SFIDISOTrigger.h> //SF_ID-ISO-Trigger
#include <DYestimate.h> //DY_DD

#ifndef __CINT__

void display_usage()
{
  std::cout << "\033[1;37musage:\033[1;m skimfile cutindex [options]" << std::endl;
  std::cout << "" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "    -i inputfile  Input file without .root" << std::endl;
  std::cout << "    -o name in the output file \"h_\"" << std::endl;
  std::cout << "    -s create a file with the systematic uncertainty yields" << std::endl;
  std::cout << "    -tr SF Trigger Uncertainty" << std::endl;
  std::cout << "    -idiso SF ID/ISO Uncertainty" << std::endl;
  std::cout << "    -d Input file directory. Default directory: InputTrees" << std::endl;
  std::cout << "    -h                 displays this help message and exits " << std::endl;
  std::cout << "" << std::endl;
}


// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const TString currentDateTime() {
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
  // for more information about date/time format
  strftime(buf, sizeof(buf), "%Y-%m-%d at %X", &tstruct);

  return buf;
}

int main(int argc, const char* argv[]){

  gSystem->Load("libTree");

  bool   _syst      = false;
  bool	 _tr_unc    = false;
  bool	 _idiso_unc = false;
  const char * _output   = 0;
  const char * _input    = 0;
  // TopTrees directory
  const char * _dir      = "/home/brochero/ttbar/TopTrees_Legacy/";
  const char * _tr       = 0;
  const char * _idiso    = 0;

  // Arguments used
  //std::set<int> usedargs;
  //Parsing input options
  if(argc == 1){
    display_usage();
    return -1;
  }

  else{
      //Argumet 1 must be a valid input fileName
      for (int i = 1; i < argc; i++){
	if( strcmp(argv[i],"-i") == 0 ){
	  _input = argv[i+1];
	  i++;
	}
	if( strcmp(argv[i],"-d") == 0 ){
	  _dir = argv[i+1];
	  i++;
	}
	if( strcmp(argv[i],"-o") == 0 ){
	  _output= argv[i+1];
	  i++;
	}
	if( strcmp(argv[i],"-s") == 0 ){
	  _syst= true;
	}
	if( strcmp(argv[i],"-tr") == 0 ){
	  _tr_unc= true;
	  _tr= argv[i+1];
	  i++;
	}
	if( strcmp(argv[i],"-idiso") == 0 ){
	  _idiso_unc= true;
	  _idiso= argv[i+1];
	  i++;
	}
	if( strcmp(argv[i],"-h") == 0 ||
	    strcmp(argv[i],"--help") == 0 ){
	  display_usage();
	  return 0;
	}
      }
  }//else
  if( _input ==0 ){
    std::cerr << "\033[1;31mskimfile ERROR:\033[1;m The '-i' option is mandatory!"
	      << std::endl;
    display_usage();
    return -1;
  }
  
  // reassigning
  TString fname(_input);
  TString hname(_output);
  TString fdir(_dir);
  TString TrUnc(_tr);
  TString IDISOUnc(_idiso);

  
  TChain theTree("AnalysisTree"); 
  
  std::cout << "-----------------------------------------" << std::endl;
  std::cout << "-----------------------------------------" << std::endl;
  std::cout << "Signal: ";
  std::cout << fname + ".root" << std::endl;
  
  theTree.Add(fdir + fname + ".root");
  
  int Event,Type,PV;
  float PUWeight;
  float MET,MET_Phi,METSig;
  float Lep0Px,Lep0Py,Lep0Pz,Lep0E,Lep1Px,Lep1Py,Lep1Pz,Lep1E;
  float NJets,HT,NBtagJets,Btag_j0,Btag_j1;
  float Jet0Px,Jet0Py,Jet0Pz,Jet0E,Jet0Et,Jet1Px,Jet1Py,Jet1Pz,Jet1E,Jet1Et;
  float BtagJet0Px,BtagJet0Py,BtagJet0Pz,BtagJet0E,BtagJet0Et,BtagJet1Px,BtagJet1Py,BtagJet1Pz,BtagJet1E,BtagJet1Et;

  float tPx,tPy,tPz,tE;
  float tbarPx,tbarPy,tbarPz,tbarE;

  float SysVarJet0,SysVarJet1,SysVarBtagJet0,SysVarBtagJet1;
  float UncJet0,UncJet1,UncBtagJet0,UncBtagJet1;
  
  theTree.SetBranchAddress( "TEvent",   &Event );
  theTree.SetBranchAddress( "TWeight",  &PUWeight );
  theTree.SetBranchAddress( "TChannel", &Type );
  theTree.SetBranchAddress( "TNPV",     &PV );

  theTree.SetBranchAddress( "TMET",     &MET );
  theTree.SetBranchAddress( "TMET_Phi", &MET_Phi );
  theTree.SetBranchAddress( "TMETSig",  &METSig );

  theTree.SetBranchAddress( "TLep0Px", &Lep0Px );
  theTree.SetBranchAddress( "TLep0Py", &Lep0Py );
  theTree.SetBranchAddress( "TLep0Pz", &Lep0Pz );
  theTree.SetBranchAddress( "TLep0E",  &Lep0E );

  theTree.SetBranchAddress( "TLep1Px", &Lep1Px );
  theTree.SetBranchAddress( "TLep1Py", &Lep1Py );
  theTree.SetBranchAddress( "TLep1Pz", &Lep1Pz );
  theTree.SetBranchAddress( "TLep1E",  &Lep1E );

  theTree.SetBranchAddress( "TNJets",    &NJets );
  theTree.SetBranchAddress( "THT",       &HT );
  theTree.SetBranchAddress( "TNJetsBtag",&NBtagJets );
  theTree.SetBranchAddress( "TBtagJet0", &Btag_j0 );
  theTree.SetBranchAddress( "TBtagJet1", &Btag_j1 );

  theTree.SetBranchAddress( "TJet0Px", &Jet0Px );
  theTree.SetBranchAddress( "TJet0Py", &Jet0Py );
  theTree.SetBranchAddress( "TJet0Pz", &Jet0Pz );
  theTree.SetBranchAddress( "TJet0E", &Jet0E );
  theTree.SetBranchAddress( "TJet0Et", &Jet0Et );

  theTree.SetBranchAddress( "TJet1Px", &Jet1Px );
  theTree.SetBranchAddress( "TJet1Py", &Jet1Py );
  theTree.SetBranchAddress( "TJet1Pz", &Jet1Pz );
  theTree.SetBranchAddress( "TJet1E", &Jet1E );  
  theTree.SetBranchAddress( "TJet1Et", &Jet1Et );

  theTree.SetBranchAddress( "TBtagJet0Px", &BtagJet0Px );
  theTree.SetBranchAddress( "TBtagJet0Py", &BtagJet0Py );
  theTree.SetBranchAddress( "TBtagJet0Pz", &BtagJet0Pz );
  theTree.SetBranchAddress( "TBtagJet0E", &BtagJet0E );
  theTree.SetBranchAddress( "TBtagJet0Et", &BtagJet0Et );

  theTree.SetBranchAddress( "TBtagJet1Px", &BtagJet1Px );
  theTree.SetBranchAddress( "TBtagJet1Py", &BtagJet1Py );
  theTree.SetBranchAddress( "TBtagJet1Pz", &BtagJet1Pz );
  theTree.SetBranchAddress( "TBtagJet1E", &BtagJet1E );
  theTree.SetBranchAddress( "TBtagJet1Et", &BtagJet1Et );

  if(fname.Contains("TT")){

    std::cout << "Reading pT reweight branches....." << std::endl;
    theTree.SetBranchAddress( "TtPx", &tPx );
    theTree.SetBranchAddress( "TtPy", &tPy );
    theTree.SetBranchAddress( "TtPz", &tPz );
    theTree.SetBranchAddress( "TtE", &tE );

    theTree.SetBranchAddress( "TtbarPx", &tbarPx );
    theTree.SetBranchAddress( "TtbarPy", &tbarPy );
    theTree.SetBranchAddress( "TtbarPz", &tbarPz );
    theTree.SetBranchAddress( "TtbarE",  &tbarE );

  }
  else{
    tPx=0.0;
    tPy=0.0;
    tPz=0.0;
    tE =0.0;

    tbarPx=0.0;
    tbarPy=0.0;
    tbarPz=0.0;
    tbarE =0.0;
  }

  if(fname.Contains("JES_Up") ||
     fname.Contains("JES_Down") ||
     fname.Contains("JER_Up") || 
     fname.Contains("JER_Down")){

    theTree.SetBranchAddress( "TSysVarJet0", &SysVarJet0 );
    theTree.SetBranchAddress( "TSysVarJet1", &SysVarJet1 );
    theTree.SetBranchAddress( "TSysVarBtagJet0", &SysVarBtagJet0 );
    theTree.SetBranchAddress( "TSysVarBtagJet1", &SysVarBtagJet1 );

    theTree.SetBranchAddress( "TUncJet0", &UncJet0 );
    theTree.SetBranchAddress( "TUncJet1", &UncJet1 );
    theTree.SetBranchAddress( "TUncBtagJet0", &UncBtagJet0 );
    theTree.SetBranchAddress( "TUncBtagJet1", &UncBtagJet1 );

  }

  else {

    SysVarJet0=1.0;
    SysVarJet1=1.0;
    SysVarBtagJet0=1.0;
    SysVarBtagJet1=1.0;

    UncJet0=1.0;
    UncJet1=1.0;
    UncBtagJet0=1.0;
    UncBtagJet1=1.0;

  }  
  
  /*********************************
             Histograms
  **********************************/
  
  TH1F *hPV[4][3];
  TH1F *hMET[4][3],*hMET_Phi[4][3],*hMETSig[4][3];
  TH1F *hInvMass[4][3],*hDiLepPt[4][3],*hLep0Pt[4][3],*hLep1Pt[4][3],*hLep0Eta[4][3],*hLep1Eta[4][3],*hDelLepPhi[4][3],*hLep0Phi[4][3],*hLep1Phi[4][3];
  TH1F *hNJets[4][3],*hHT[4][3],*hNBtagJets[4][3],*hNBtagJets5[4][3],*hJet0Pt[4][3],*hJet1Pt[4][3],*hBtagJet0Pt[4][3];

  TH1F *hDYIn[4][3],*hDYOut[4][3],*hDYIn_btag0[4][3],*hDYOut_btag0[4][3],*hDYIn_btag1[4][3],*hDYOut_btag1[4][3];
  TH2F *hDY_NPV[4][3];
  TH2F *hDY_NJets[4][3];
  TH1F *hDY_TF_InvMass[4][3];

  TH1F *hSFpT[4][3],*hSFpTError[4][3];

  TH1F *hSFIDISO[4][3],*hSFIDISOError[4][3];
  TH1F *hSFIDISO_mu[4],*hSFIDISOError_mu[4];
  TH1F *hSFIDISO_e[4],*hSFIDISOError_e[4];
  TH1F *hSFTrigger[4][3],*hSFTriggerError[4][3];

  TH1F *hSysVarJet0[4][3]        ,*hUncJet0[4][3];
  TH1F *hSysVarJet1[4][3]        ,*hUncJet1[4][3];
  TH1F *hSysVarBtagJet0[4][3]    ,*hUncBtagJet0[4][3];
  TH2F *hSysVarJet0Pt[4][3]      ,*hUncJet0Pt[4][3];
  TH2F *hSysVarJet1Pt[4][3]      ,*hUncJet1Pt[4][3];
  TH2F *hSysVarBtagJet0Pt[4][3]  ,*hUncBtagJet0Pt[4][3];

  TH2F *h2DSFIDISO_mu[4]  ,*h2DSFIDISO_e[4];

  TString namech[3];
  namech[0]="mumu";
  namech[1]="ee";
  namech[2]="mue";

  TString namecut[4];
  namecut[0]="dilepton";
  namecut[1]="2Jets";
  namecut[2]="MET";
  namecut[3]="1btag";

  TString titlenamech[3];
  titlenamech[0]="#mu#mu";
  titlenamech[1]="ee";
  titlenamech[2]="e#mu";
  
  for(int j=0; j<4; j++){   // Cut
    for(int i=0; i<3; i++){ // Channel
      hPV[j][i]         = new TH1F("hPV_"+namech[i]+"_"+namecut[j],"PV Distribution  " + titlenamech[i],30,0,30);
      
      //hMET[j][i]        = new TH1F("hMET_"+namech[i]+"_"+namecut[j],"#slash{E}_{T} " + titlenamech[i],80,0,400);
      hMET[j][i]        = new TH1F("hMET_"+namech[i]+"_"+namecut[j],"#slash{E}_{T} " + titlenamech[i],40,0,200);
      hMET_Phi[j][i]    = new TH1F("hMET_Phi_"+namech[i]+"_"+namecut[j],"#Phi_{#slash{E}_{T}} " + titlenamech[i],160,-4,4);
      hMETSig[j][i]     = new TH1F("hMETSig_"+namech[i]+"_"+namecut[j],"#slash{E}_{T} Significance " + titlenamech[i],50,0,150);
      
      if(j==0) hInvMass[j][i] = new TH1F("hInvMass_"+namech[i]+"_"+namecut[j],"Invariant mass " + titlenamech[i],60,0.0,300.0);
      else hInvMass[j][i]     = new TH1F("hInvMass_"+namech[i]+"_"+namecut[j],"Invariant mass " + titlenamech[i],60,6.0,306.0);
      hDiLepPt[j][i]    = new TH1F("hDiLepPt_"+namech[i]+"_"+namecut[j],"Dilepton p_{T} " + titlenamech[i],50,0,250);
      hLep0Pt[j][i]     = new TH1F("hLep0Pt_"+namech[i]+"_"+namecut[j],"p_{T} leading lepton " + titlenamech[i],50,0.0,250.0);
      hLep1Pt[j][i]     = new TH1F("hLep1Pt_"+namech[i]+"_"+namecut[j],"p_{T} second leading lepton " + titlenamech[i],50,0.0,250.0);
      hLep0Eta[j][i]    = new TH1F("hLep0Eta_"+namech[i]+"_"+namecut[j],"#eta_{Lep_{0}} " + titlenamech[i],160,-8,8);
      hLep1Eta[j][i]    = new TH1F("hLep1Eta_"+namech[i]+"_"+namecut[j],"#eta_{Lep_{1}} " + titlenamech[i],160,-8,8);
      //hDelLepPhi[j][i]  = new TH1F("hDelLepPhi_"+namech[i]+"_"+namecut[j],"#Delta#phi_{ll} " + titlenamech[i],50,-3.2,3.2);
      hDelLepPhi[j][i]  = new TH1F("hDelLepPhi_"+namech[i]+"_"+namecut[j],"#Delta#phi_{ll} " + titlenamech[i],25,0.0,3.2);
      hLep0Phi[j][i]    = new TH1F("hLep0Phi_"+namech[i]+"_"+namecut[j],"#phi_{Lep_{0}} " + titlenamech[i],100,-5,5);
      hLep1Phi[j][i]    = new TH1F("hLep1Phi_"+namech[i]+"_"+namecut[j],"#phi_{Lep_{1}} " + titlenamech[i],100,-5,5);
      
      hNJets[j][i]      = new TH1F("hNJets_"+namech[i]+"_"+namecut[j],"Jet multiplicity " + titlenamech[i],5,-0.5,4.5);
      hNJets[j][i]->GetXaxis()->SetBinLabel(1,"0");
      hNJets[j][i]->GetXaxis()->SetBinLabel(2,"1");
      hNJets[j][i]->GetXaxis()->SetBinLabel(3,"2");
      hNJets[j][i]->GetXaxis()->SetBinLabel(4,"3");
      hNJets[j][i]->GetXaxis()->SetBinLabel(5,"#geq 4");

      hNBtagJets5[j][i]  = new TH1F("hNBtagJets5_"+namech[i]+"_"+namecut[j],"b-tag jet multiplicity " + titlenamech[i],5,-0.5,4.5);
      hNBtagJets5[j][i]->GetXaxis()->SetBinLabel(1,"0");
      hNBtagJets5[j][i]->GetXaxis()->SetBinLabel(2,"1");
      hNBtagJets5[j][i]->GetXaxis()->SetBinLabel(3,"2");
      hNBtagJets5[j][i]->GetXaxis()->SetBinLabel(4,"3");
      hNBtagJets5[j][i]->GetXaxis()->SetBinLabel(5,"#geq 4");

      hNBtagJets[j][i]  = new TH1F("hNBtagJets_"+namech[i]+"_"+namecut[j],"b-tag jet multiplicity " + titlenamech[i],4,-0.5,3.5);
      hNBtagJets[j][i]->GetXaxis()->SetBinLabel(1,"0");
      hNBtagJets[j][i]->GetXaxis()->SetBinLabel(2,"1");
      hNBtagJets[j][i]->GetXaxis()->SetBinLabel(3,"2");
      hNBtagJets[j][i]->GetXaxis()->SetBinLabel(4,"#geq 3");


      hHT[j][i]         = new TH1F("hHT_"+namech[i]+"_"+namecut[j],"H_{T} " + titlenamech[i],300,0,600);
      hJet0Pt[j][i]     = new TH1F("hJet0Pt_"+namech[i]+"_"+namecut[j],"p_{T} leading jet " + titlenamech[i],50,0,250);
      hJet1Pt[j][i]     = new TH1F("hJet1Pt_"+namech[i]+"_"+namecut[j],"p_{T} Second leading jet " + titlenamech[i],50,0,250);
      hBtagJet0Pt[j][i] = new TH1F("hBtagJet0Pt_"+namech[i]+"_"+namecut[j],"p_{T} leading b-tagged jet " + titlenamech[i],50,0,250);
      
      
      /***************************
                 DY-DD
      ***************************/
      hDYIn[j][i]           = new TH1F("hDYIn_"+namech[i]+"_"+namecut[j],"DY In " + titlenamech[i],100,0,1000);    
      hDYOut[j][i]          = new TH1F("hDYOut_"+namech[i]+"_"+namecut[j],"DY Out " + titlenamech[i],100,0,1000);    

      hDYIn_btag0[j][i]           = new TH1F("hDYIn_btag0_"+namech[i]+"_"+namecut[j],"DY In " + titlenamech[i] + " btag==0",100,0,1000);    
      hDYOut_btag0[j][i]          = new TH1F("hDYOut_btag0_"+namech[i]+"_"+namecut[j],"DY Out " + titlenamech[i] + " btag==0",100,0,1000);    

      hDYIn_btag1[j][i]           = new TH1F("hDYIn_btag1_"+namech[i]+"_"+namecut[j],"DY In " + titlenamech[i] + " btag>=1",100,0,1000);    
      hDYOut_btag1[j][i]          = new TH1F("hDYOut_btag1_"+namech[i]+"_"+namecut[j],"DY Out " + titlenamech[i] + " btag>=1",100,0,1000);    

      hDY_NPV[j][i]         = new TH2F("hDY_vs_NPV_"+namech[i]+"_"+namecut[j],"DY vs NPV" + titlenamech[i],750,0.,750.,31,-0.5,30.5);    
      hDY_NJets[j][i]       = new TH2F("hDY_vs_NJets_"+namech[i]+"_"+namecut[j],"DY vs NJets" + titlenamech[i],750,0.,750.,5,-0.5,4.5);

      hDY_TF_InvMass[j][i]  = new TH1F("hDY_TF_InvMass_"+namech[i]+"_"+namecut[j],"(DY-TF) Invariant Mass " + titlenamech[i],60,6.0,306.0);

      /***************************
          SF(ID,ISO & Trigger)
      ***************************/
      hSFIDISO[j][i]           = new TH1F("hSFIDISO_"+namech[i]+"_"+namecut[j],"SF_{ID,ISO} " + titlenamech[i],400,0.8,1.2);    
      hSFIDISOError[j][i]      = new TH1F("hSFIDISOError_"+namech[i]+"_"+namecut[j],"#Delta SF_{ID,ISO} " + titlenamech[i],400,0,0.05); 
      hSFTrigger[j][i]         = new TH1F("hSFTrigger_"+namech[i]+"_"+namecut[j],"SF^{Trigger} " + titlenamech[i],400,0.8,1.2);    
      hSFTriggerError[j][i]    = new TH1F("hSFTriggerError_"+namech[i]+"_"+namecut[j],"#Delta SF^{Trigger} " + titlenamech[i],400,0,0.05);


      /***************************
          SF(pT Reweight)
      ***************************/
      hSFpT[j][i]           = new TH1F("hSFpT_"+namech[i]+"_"+namecut[j],"SF_{pT} " + titlenamech[i],500,0.4,1.4);    
      hSFpTError[j][i]      = new TH1F("hSFpTError_"+namech[i]+"_"+namecut[j],"#Delta SF_{pT} " + titlenamech[i],400,0,0.05); 

 
      /***************************
              Systematics
      ***************************/
      hSysVarJet0[j][i]       = new TH1F("hSysVarJet0_"+namech[i]+"_"+namecut[j],"Systematic Variation: Jet0 " + titlenamech[i],100,0.9,1.1);    
      hSysVarJet1[j][i]       = new TH1F("hSysVarJet1_"+namech[i]+"_"+namecut[j],"Systematic Variation: Jet1 " + titlenamech[i],100,0.9,1.1);    
      hSysVarBtagJet0[j][i]   = new TH1F("hSysVarBtagJet0_"+namech[i]+"_"+namecut[j],"Systematic Variation: BtagJet0 " + titlenamech[i],100,0.9,1.1);    
      
      hUncJet0[j][i]          = new TH1F("hUncJet0_"+namech[i]+"_"+namecut[j],"Jet0 Uncertainty " + titlenamech[i],100,0.0,2);    
      hUncJet1[j][i]          = new TH1F("hUncJet1_"+namech[i]+"_"+namecut[j],"Jet1 Uncertainty " + titlenamech[i],100,0.0,2);    
      hUncBtagJet0[j][i]      = new TH1F("hUncBtagJet0_"+namech[i]+"_"+namecut[j],"BtagJet0 Uncertainty " + titlenamech[i],100,0.0,2);

      hSysVarJet0Pt[j][i]     = new TH2F("hSysVarJet0Pt_"+namech[i]+"_"+namecut[j],"Systematic Variation Vs P_{t}^{Jet0} " + titlenamech[i],120,0.0,120.0,100,0.9,1.1);    
      hSysVarJet1Pt[j][i]     = new TH2F("hSysVarJet1Pt_"+namech[i]+"_"+namecut[j],"Systematic Variation Vs P_{t}^{Jet1} " + titlenamech[i],120,0.0,120.0,100,0.9,1.1);    
      hSysVarBtagJet0Pt[j][i] = new TH2F("hSysVarBtagJet0Pt_"+namech[i]+"_"+namecut[j],"Systematic Variation Vs P_{t}^{BtagJet0} " + titlenamech[i],120,0.0,120,100,0.9,1.1);    

      hUncJet0Pt[j][i]        = new TH2F("hUncJet0Pt_"+namech[i]+"_"+namecut[j],"Jet Uncertainty Vs P_{t}^{Jet0} " + titlenamech[i],120,0.0,120.0,100,0.0,2);    
      hUncJet1Pt[j][i]        = new TH2F("hUncJet1Pt_"+namech[i]+"_"+namecut[j],"Jet Uncertainty Vs P_{t}^{Jet1} " + titlenamech[i],120,0.0,120.0,100,0.0,2);    
      hUncBtagJet0Pt[j][i]    = new TH2F("hUncBtagJet0Pt_"+namech[i]+"_"+namecut[j],"Jet Uncertainty Vs P_{t}^{BtagJet0} " + titlenamech[i],120,0.0,120.0,100,0.0,2);    

    }//for(i)


    hSFIDISO_mu[j]        = new TH1F("hSFIDISO_mu_"+namecut[j],"SF_{ID,ISO}^{#mu} ",400,0.9,1.1);    
    hSFIDISOError_mu[j]   = new TH1F("hSFIDISOError_mu_"+namecut[j],"#Delta SF_{ID,ISO}^{mu} ",300,0,0.03); 

    hSFIDISO_e[j]         = new TH1F("hSFIDISO_e_"+namecut[j],"SF_{ID,ISO}^{e} ",400,0.9,1.1);    
    hSFIDISOError_e[j]    = new TH1F("hSFIDISOError_e_"+namecut[j],"#Delta SF_{ID,ISO}^{e} ",300,0,0.03); 


    float hbinmuEta[7]={-2.4 , -1.2 , -0.9 , 0.0 , 0.9 , 1.2 , 2.4}; // muon Eta values
    float hbinmuPt[5] ={20 , 30 , 40 , 60 , 200}; // muon Pt values      
    
    h2DSFIDISO_mu[j]    = new TH2F("h2DSFIDISO_muon_"+namecut[j],"SF_{ID,ISO}^{#mu}(#eta,p_{t}) ",6,hbinmuEta,4,hbinmuPt);
    
    float hbineEta[11]={-2.5 , -2.0 , -1.556 , -1.442 , -0.8 , 0.0 , 0.8 , 1.442 , 1.556 , 2.0 , 2.5}; // electron Eta values
    float hbinePt[5] ={20 , 30 , 40 , 50 , 200}; // electron Pt values
    
    h2DSFIDISO_e[j]    = new TH2F("h2DSFIDISO_e_"+namecut[j],"SF_{ID,ISO}^{e}(#eta,p_{t}) ",10,hbineEta,4,hbinePt);
    
  }//for(j)

  TLorentzVector Lep0,Lep1;
  TLorentzVector Jet0,Jet1,BtagJet0,BtagJet1;

  TStopwatch sw;
  sw.Start(kTRUE);

  ///////////////////////////////////////
  // Please, IGNORE. Temporal solution //
  ///////////////////////////////////////
  TCanvas *mydummycanvas=new TCanvas();// 
  ///////////////////////////////////////
  
  
  /*******************
   SF Parametrization
  ******************/
  
  TH2F *hmuIDSF, *hmumuTriggerSF;
  TH2F *heIDSF, *heeTriggerSF;
  TH2F *hmueTriggerSF;  

  // Lepton SFs: ID and Iso w/ stat. + syst. Errors
  TFile *MuSF   = TFile::Open(fdir + "SF_muon.root"); 
  TFile *ElSF   = TFile::Open(fdir + "SF_electron.root"); 
  TFile *MuElSF = TFile::Open(fdir + "SF_Trigger_muon-electron.root"); 

  if(!MuSF || !ElSF || !MuElSF){
    std::cerr << "ERROR [SF]: Could not open SF files!!!"  << std::endl;
    std::exit(0);
  }

  hmuIDSF = (TH2F*) MuSF->Get("GlobalSF")->Clone("muIDSF");
  hmumuTriggerSF = (TH2F*) MuSF->Get("scalefactor_eta2d_with_syst")->Clone("mumuTriggerSF"); // PromptReco
  if(!hmuIDSF || !hmumuTriggerSF){
    std::cerr << "ERROR [MuonSF]: Could not find histogram for SF reweighting" << std::endl;
  }

  heIDSF = (TH2F*) ElSF->Get("GlobalSF")->Clone("eIDSF");
  heeTriggerSF = (TH2F*) ElSF->Get("scalefactor_eta2d_with_syst")->Clone("eeTriggerSF"); // PromptReco
  if(!heIDSF || !heeTriggerSF){
    std::cerr << "ERROR [ElectronSF]: Could not find histogram for SF reweighting" << std::endl;
  }

  hmueTriggerSF = (TH2F*) MuElSF->Get("scalefactor_eta2d_with_syst")->Clone("mueTriggerSF"); // PromptReco
  if(!hmueTriggerSF){
    std::cerr << "ERROR [MuonElectronSF]: Could not find histogram for SF reweighting" << std::endl;
  }

  /********************************************
   Non W/Z Scale
  *********************************************/
  //             [bin][cut][channel]
  float NonWZ_DD_bin[4][4][3];
  
  for(int jbin=0; jbin<5; jbin++){
    for(int jcut=0; jcut<5; jcut++){
      for(int jchannel=0; jchannel<4; jchannel++){
	NonWZ_DD_bin[jbin][jcut][jchannel]=1.0; // NO SF
      }
    }
  }

  // [bin][cut][channel]
  // v8 04/07/13 (mumu)
  // MET
  //NonWZ_DD_bin[0][2][0]=3.99;
  //NonWZ_DD_bin[1][2][0]=4.75;
  // v8 04/07/13 (ee)
  // MET
  //NonWZ_DD_bin[0][2][1]=0.73;
  //NonWZ_DD_bin[1][2][1]=0.25;
  // v8 04/07/13 (mue)
  // MET
  //NonWZ_DD_bin[0][2][2]=2.26;
  //NonWZ_DD_bin[1][2][2]=3.35;



  /********************************************
   DY-DD Scale Factors/bin(btag) and Cut level
  *********************************************/

  // [cut:4][channel:3][bin:3 (0=0 btag, 1=1 btag and 2=global)][R_var:8]
  float *pDY_DD_R[4][3][3][8], DY_DD_R[4][3][3][8];

  for(int R_cut=0;R_cut<4;R_cut++){
    for(int R_ch=0;R_ch<3;R_ch++){      
      for(int R_bin=0;R_bin<3;R_bin++){      
	for(int R_var=0;R_var<8;R_var++){    
  
	  DY_DD_R [R_cut][R_ch][R_bin][R_var] = 0.0;
	  pDY_DD_R[R_cut][R_ch][R_bin][R_var] = & DY_DD_R[R_cut][R_ch][R_bin][R_var];

	}
      }
    }
  }

  if(fname.Contains("ZDY")){

  std::cout << "\n----------------------------------- " << std::endl;
  std::cout << "--- Estimating SF_DY using R_out/in... " << std::endl;
  std::cout << "----------------------------------- " << std::endl;


  TString R_ch_name[3];
  R_ch_name[0]="btag0_";
  R_ch_name[1]="btag1_";
  R_ch_name[2]="";

  for(int R_cut=0;R_cut<4;R_cut++){
    for(int R_bin=0;R_bin<3;R_bin++){      
      // SF
      DYestimateValue(R_ch_name[R_bin] + "mumu",namecut[R_cut], pDY_DD_R[R_cut][0][R_bin]);
      DYestimateValue(R_ch_name[R_bin] + "ee"  ,namecut[R_cut], pDY_DD_R[R_cut][1][R_bin]);
      // OF
      if(R_cut==2)                  DY_DD_R[R_cut][2][R_bin][4] = DY_DD_R[1][2][R_bin][4]; // OF MET (Not MET cut)
      else if(R_cut==3 && R_bin!=0) DY_DD_R[R_cut][2][R_bin][4] = sqrt(DY_DD_R[1][0][1][4]*DY_DD_R[1][1][1][4]); // OF >= 1btag (Not MET cut)
      else                          DY_DD_R[R_cut][2][R_bin][4] = sqrt(DY_DD_R[R_cut][0][R_bin][4]*DY_DD_R[R_cut][1][R_bin][4]);

      if (R_cut==2 && R_bin==1){
	for(int n=0; n<7; n++){
	  std::cout << "SF[" << n << "]= " << DY_DD_R[R_cut][0][R_bin][n] << std::endl; 
	}
      }
      std::cout << "Cut= " << R_cut << "  Bin= " << R_bin << std::endl;
      std::cout << "\t SF_mue=" << DY_DD_R[R_cut][2][R_bin][4] << "\t SF_mumu=" << DY_DD_R[R_cut][0][R_bin][4] << "\t SF_ee=" << DY_DD_R[R_cut][1][R_bin][4] << std::endl;
    }
  }

  std::cout << "-----------------------------------" << std::endl;
  std::cout << "--- SF_DY using R_out/in estimated! " << std::endl;
  std::cout << "----------------------------------- \n" << std::endl;

  }// if(ZDY)

  // Number de events for <pT Reweight>
  //          [Cut][Channel]
  float SF_pTweight[4][3]={0,0,0,0,
			   0,0,0,0,
			   0,0,0,0};
  // Number de events for acceptance
  //          [Cut][Channel]
  int AccEvent[4][3]={0,0,0,0,
		      0,0,0,0,
		      0,0,0,0};
  // Number de events for acceptance
  //          [Cut][Channel]
  float EffEvent[4][3]={0.0,0.0,0.0,0.0,
			0.0,0.0,0.0,0.0,
			0.0,0.0,0.0,0.0};
  


  std::cout << "--- Processing: " << theTree.GetEntries() << " events" << std::endl;
  for (Long64_t ievt=0; ievt<theTree.GetEntries();ievt++) {
     
    theTree.GetEntry(ievt);
  
    //////////////////////////////////////////////////////
    if (ievt%100000 == 0){ 
      float progress=(ievt)/(theTree.GetEntries()*1.0);
      int barWidth = 50;
      
      std::cout << "[";
      int pos = barWidth * progress;
    
      for (int i = 0; i < barWidth; ++i) {
      	if (i < pos) std::cout << "=";
      	else if (i == pos) std::cout << ">";
      	else std::cout << " ";
      }
      
      std::cout << "] " << int(progress * 100.0) << " %\r";
      std::cout.flush();
    }
    ////////////////////////////////////////////////////////
    
  
    /*************************
      Acceptance: GEN level
    **************************/
    if(Type==3 || Type==4 || Type==5 || Type==-5) continue; // RECO Process
    
    // if(Type==0 || Type==1 || Type==2 || Type==-2) continue; // GEN Process
    // else if(Type== 3) Type=0;
    // else if(Type== 4) Type=1;
    // else if(Type== 5) Type=2;
    // else if(Type==-5) Type=-2;
    
    //emu=2;mue=-2
    int channel=Type; // SF/bin
    if(Type==-2) Type=2;// Fill Histograms MuonElectron=ElectronMuon    
    
    Lep0.SetPxPyPzE(Lep0Px,Lep0Py,Lep0Pz,Lep0E);
    Lep1.SetPxPyPzE(Lep1Px,Lep1Py,Lep1Pz,Lep1E);
    
    float InvMass=(Lep0+Lep1).M();       

    Jet0.SetPxPyPzE(Jet0Px*SysVarJet0,Jet0Py*SysVarJet0,Jet0Pz,Jet0E);
    Jet1.SetPxPyPzE(Jet1Px*SysVarJet1,Jet1Py*SysVarJet1,Jet1Pz,Jet1E);
    
    BtagJet0.SetPxPyPzE(BtagJet0Px*SysVarBtagJet0,BtagJet0Py*SysVarBtagJet0,BtagJet0Pz,BtagJet0E);
    BtagJet1.SetPxPyPzE(BtagJet1Px*SysVarBtagJet1,BtagJet1Py*SysVarBtagJet1,BtagJet1Pz,BtagJet1E);
    
    bool ZVeto=false;
    bool ZMass=false;
    if(InvMass<76. || InvMass>106.) ZVeto=true; 
    if(InvMass>20.)                 ZMass=true;
      
  
  /*******************************************
   Trigger,ID & ISO Scale Factors/bin(Pt,Eta)
  *******************************************/
  
  std::vector<float> SF_ID_ISO_Tr;
  
  if (fname.Contains("Data")){
    SF_ID_ISO_Tr.push_back(1.0);//SF_ID_ISO_Tr    [0] 
    SF_ID_ISO_Tr.push_back(1.0);//SF_ID_ISO       [1] 
    SF_ID_ISO_Tr.push_back(1.0);//SF_ID_ISO_Error [2] 
    SF_ID_ISO_Tr.push_back(1.0);//SF_Tr           [3] 
    SF_ID_ISO_Tr.push_back(1.0);//SF_Tr_Error     [4] 
    SF_ID_ISO_Tr.push_back(1.0);//SF_Lep0         [5] 
    SF_ID_ISO_Tr.push_back(1.0);//SF_Lep0_Error   [6] 
    SF_ID_ISO_Tr.push_back(1.0);//SF_Lep1         [7] 
    SF_ID_ISO_Tr.push_back(1.0);//SF_Lep1_Error   [8] 
  }
  
  else {
    SFIDISOTrigger(SF_ID_ISO_Tr,
    		   Lep0, Lep1,channel,
    		   hmuIDSF, hmumuTriggerSF,
    		   heIDSF, heeTriggerSF,
    		   hmueTriggerSF);

    if(_idiso_unc){
      
      if(IDISOUnc=="Up"){
	PUWeight=PUWeight*(SF_ID_ISO_Tr[1] + SF_ID_ISO_Tr[2]);	
      }
      
      else if(IDISOUnc=="Down"){
	PUWeight=PUWeight*(SF_ID_ISO_Tr[1] - SF_ID_ISO_Tr[2]);	
      }

      else if(IDISOUnc=="Central"){
	PUWeight=PUWeight*(SF_ID_ISO_Tr[1]);
      }
    } // if(_idiso_unc)

    else if(_tr_unc){

      if(TrUnc=="Up"){
	PUWeight=PUWeight*(SF_ID_ISO_Tr[3] + SF_ID_ISO_Tr[4]);			
      }
      
      else if(TrUnc=="Down"){
	PUWeight=PUWeight*(SF_ID_ISO_Tr[3] - SF_ID_ISO_Tr[4]);		
      }

      else if(TrUnc=="Central"){
	PUWeight=PUWeight*(SF_ID_ISO_Tr[3]);	
      }
    }// if(_tr_unc) 
      
    else PUWeight=PUWeight*(SF_ID_ISO_Tr[0]); 
    
  }// else(Contain("Data"))
  

    /*******************************************
      DY Data Driven: Rout/in and Template Fit
    *******************************************/

    int cut=-1;

    //(DY_DD)/bin
    // Every cut SHOULD include the previous one!
    if(ZMass)                                         cut=0; // Dileptons
    if(ZMass && NJets>1)                              cut=1; // + 2 Jets
    if(ZMass && NJets>1 && (MET>40.0))                cut=2; // + MET
    if(ZMass && NJets>1 && (MET>40.0) && NBtagJets>0) cut=3; // + 1 Btag
    
    
    for(int icut=0; icut<cut+1; icut++){
      // CF mumu and ee (Only Dilepton level)
      if(icut == 0)            hDY_NPV[icut][Type]  ->Fill(InvMass,PV,PUWeight);
      if(icut == 0 && MET<10 ) hDY_NJets[icut][Type]->Fill(InvMass,NJets,PUWeight);
      // Histograms DY-DD (Rout/in mumu and ee)
      if(ZVeto){
	hDYOut[icut][Type]->Fill(InvMass,PUWeight); // All	
	if(NBtagJets<1) hDYOut_btag0[icut][Type]->Fill(InvMass,PUWeight); // b-tag==0	
	if(NBtagJets>0) hDYOut_btag1[icut][Type]->Fill(InvMass,PUWeight); // b-tag=>1	
      }
      else{
	hDYIn[icut][Type] ->Fill(InvMass,PUWeight); // All
	if(NBtagJets<1) hDYIn_btag0[icut][Type] ->Fill(InvMass,PUWeight); // b-tag==0 
	if(NBtagJets>0) hDYIn_btag1[icut][Type] ->Fill(InvMass,PUWeight); // b-tag=>1
      }      
      // Histograms DY-TF (All Channels)
      if(Type==2 || ZVeto) hDY_TF_InvMass[icut][Type]->Fill(InvMass,PUWeight);
      
    }
    
 
    /***************************
            Selection
    ***************************/
    cut=-1;    
    // Every cut SHOULD include the previous one!
    if(ZMass)                                                               cut=0; // Dilepton
    if(ZMass && NJets>1 && (Type==2 || (ZVeto)))                            cut=1; // + 2Jets and ZVeto 
    if(ZMass && NJets>1 && (Type==2 || (ZVeto && MET>40.0)))                cut=2; // + MET
    if(ZMass && NJets>1 && (Type==2 || (ZVeto && MET>40.0)) && NBtagJets>0) cut=3; // + 1 Btag


    float DiLepPt=(Lep0+Lep1).Pt();       
    
    float DeltaPhiLep=Lep0.DeltaPhi(Lep1);
    float DeltaEtaLep=Lep0.Eta()-Lep1.Eta();
     
    float PUWeightNoDY=PUWeight;
    
    for(int icut=0; icut<cut+1; icut++){
      
      // PUWeight reset for each cut 
      PUWeight=PUWeightNoDY;

      /************************************************************
       DY-DD Scale Factors/bin(btag)
      *************************************************************/
      if(fname.Contains("ZDY")){
	if(NBtagJets==0) PUWeight=PUWeight*DY_DD_R[icut][Type][0][4];
	if(NBtagJets>0)  PUWeight=PUWeight*DY_DD_R[icut][Type][1][4];
      }

      /************************************************************
       Non-W/Z SF
      *************************************************************/
      if(fname.Contains("WJets") || fname.Contains("Bkg")){
	//if(NBtagJets==0) PUWeight=PUWeight*NonWZ_DD_bin[0][icut][Type];
	//if(NBtagJets>0)  PUWeight=PUWeight*NonWZ_DD_bin[1][icut][Type];
      }

      /************************************************************
       pT reweight
      *************************************************************/
      if(fname.Contains("TT")){

	// pT Top Reweight 7TeV
	TLorentzVector t,tbar;
	t.SetPxPyPzE(tPx,tPy,tPz,tE);
	tbar.SetPxPyPzE(tbarPx,tbarPy,tbarPz,tbarE);
	
	// pT Reweight: Only for Systematic Uncertainty
	// Definition from pT reweight Twiki
	// https://twiki.cern.ch/twiki/bin/viewauth/CMS/TopPtReweighting and 
	// https://twiki.cern.ch/twiki/bin/view/CMS/TopSystematicsRun1#pt_top_Reweighting
	float SF_tPt=sqrt( exp(0.222-0.00197*t.Pt()) * exp(0.222-0.00197*tbar.Pt()) );
	hSFpT[icut][Type]->Fill(SF_tPt,PUWeight); 	
	//PUWeight=PUWeight*SF_tPt;   
	SF_pTweight[icut][Type]+=SF_tPt;
      } // if(TT)

      /*************************************************************/

      /*******************
        Fill Histograms
      ******************/

      hSFIDISO[icut][Type]->Fill(SF_ID_ISO_Tr[1],PUWeight);
      hSFIDISOError[icut][Type]->Fill(SF_ID_ISO_Tr[2],PUWeight);
      hSFTrigger[icut][Type]->Fill(SF_ID_ISO_Tr[3],PUWeight);
      hSFTriggerError[icut][Type]->Fill(SF_ID_ISO_Tr[4],PUWeight);

      if(channel==0){
	hSFIDISO_mu[icut]->Fill(SF_ID_ISO_Tr[5],PUWeight);
	hSFIDISOError_mu[icut]->Fill(SF_ID_ISO_Tr[6],PUWeight);
	hSFIDISO_mu[icut]->Fill(SF_ID_ISO_Tr[7],PUWeight);
	hSFIDISOError_mu[icut]->Fill(SF_ID_ISO_Tr[8],PUWeight);

	h2DSFIDISO_mu[icut]->Fill(Lep0.Eta(),Lep0.Pt(),PUWeight);
	h2DSFIDISO_mu[icut]->Fill(Lep1.Eta(),Lep1.Pt(),PUWeight);
      }
      if(channel==1){
	hSFIDISO_e[icut]->Fill(SF_ID_ISO_Tr[5],PUWeight);
	hSFIDISOError_e[icut]->Fill(SF_ID_ISO_Tr[6],PUWeight);
	hSFIDISO_e[icut]->Fill(SF_ID_ISO_Tr[7],PUWeight);
	hSFIDISOError_e[icut]->Fill(SF_ID_ISO_Tr[8],PUWeight);

	h2DSFIDISO_e[icut]->Fill(Lep0.Eta(),Lep0.Pt(),PUWeight);
	h2DSFIDISO_e[icut]->Fill(Lep1.Eta(),Lep1.Pt(),PUWeight);
      }
      if(channel==2){
	hSFIDISO_e[icut]->Fill(SF_ID_ISO_Tr[5],PUWeight);
	hSFIDISOError_e[icut]->Fill(SF_ID_ISO_Tr[6],PUWeight);
	hSFIDISO_mu[icut]->Fill(SF_ID_ISO_Tr[7],PUWeight);
	hSFIDISOError_mu[icut]->Fill(SF_ID_ISO_Tr[8],PUWeight);

	h2DSFIDISO_e[icut] ->Fill(Lep0.Eta(),Lep0.Pt(),PUWeight);
	h2DSFIDISO_mu[icut]->Fill(Lep1.Eta(),Lep1.Pt(),PUWeight);
      }
      if(channel==-2){
	hSFIDISO_mu[icut]->Fill(SF_ID_ISO_Tr[5],PUWeight);
	hSFIDISOError_mu[icut]->Fill(SF_ID_ISO_Tr[6],PUWeight);
	hSFIDISO_e[icut]->Fill(SF_ID_ISO_Tr[7],PUWeight);
	hSFIDISOError_e[icut]->Fill(SF_ID_ISO_Tr[8],PUWeight);

	h2DSFIDISO_mu[icut]->Fill(Lep0.Eta(),Lep0.Pt(),PUWeight);
	h2DSFIDISO_e[icut] ->Fill(Lep1.Eta(),Lep1.Pt(),PUWeight);
      }
    
      /******************
          Acceptace
      ******************/
      AccEvent[icut][Type]++;
      EffEvent[icut][Type]= EffEvent[icut][Type] + PUWeight;


      /******************
        Kinematic Var.
      ******************/
      hPV[icut][Type]->Fill(PV,PUWeight);
      
      hMET[icut][Type]->Fill(MET,PUWeight);
      hMET_Phi[icut][Type]->Fill(MET_Phi,PUWeight);
      hMETSig[icut][Type]->Fill(METSig,PUWeight);
      
      hInvMass[icut][Type]->Fill(InvMass,PUWeight);
      hDiLepPt[icut][Type]->Fill(DiLepPt,PUWeight);
      hLep0Pt[icut][Type]->Fill(Lep0.Pt(),PUWeight);
      hLep1Pt[icut][Type]->Fill(Lep1.Pt(),PUWeight);
      hLep0Eta[icut][Type]->Fill(Lep0.Eta(),PUWeight);
      hLep1Eta[icut][Type]->Fill(Lep1.Eta(),PUWeight);
      hDelLepPhi[icut][Type]->Fill(fabs(DeltaPhiLep),PUWeight);
      hLep0Phi[icut][Type]->Fill(Lep0.Phi(),PUWeight);
      hLep1Phi[icut][Type]->Fill(Lep1.Phi(),PUWeight);
      
      if(NJets>4) hNJets[icut][Type]->Fill(4,PUWeight);
      else hNJets[icut][Type]->Fill(NJets,PUWeight); 
      
      if(NBtagJets>4) hNBtagJets5[icut][Type]->Fill(4,PUWeight);
      else hNBtagJets5[icut][Type]->Fill(NBtagJets,PUWeight);

      if(NBtagJets>3) hNBtagJets[icut][Type]->Fill(3,PUWeight);
      else hNBtagJets[icut][Type]->Fill(NBtagJets,PUWeight);

      hHT[icut][Type]->Fill(HT,PUWeight);
      hJet0Pt[icut][Type]->Fill(Jet0.Pt(),PUWeight);
      hJet1Pt[icut][Type]->Fill(Jet1.Pt(),PUWeight);
      hBtagJet0Pt[icut][Type]->Fill(BtagJet0.Pt(),PUWeight);

      /***************************
         Systematic Histograms
      ***************************/
      hUncJet0[icut][Type]->Fill(UncJet0,PUWeight);
      hUncJet1[icut][Type]->Fill(UncJet1,PUWeight);
      hUncBtagJet0[icut][Type]->Fill(UncBtagJet0,PUWeight);

      hSysVarJet0[icut][Type]->Fill(SysVarJet0,PUWeight);
      hSysVarJet1[icut][Type]->Fill(SysVarJet1,PUWeight);
      hSysVarBtagJet0[icut][Type]->Fill(SysVarBtagJet0,PUWeight);

      hUncJet0Pt[icut][Type]->Fill(Jet0.Pt(),UncJet0);
      hUncJet1Pt[icut][Type]->Fill(Jet1.Pt(),UncJet1);
      hUncBtagJet0Pt[icut][Type]->Fill(BtagJet0.Pt(),UncBtagJet0);

      hSysVarJet0Pt[icut][Type]->Fill(Jet0.Pt(),SysVarJet0);
      hSysVarJet1Pt[icut][Type]->Fill(Jet1.Pt(),SysVarJet1);
      hSysVarBtagJet0Pt[icut][Type]->Fill(BtagJet0.Pt(),SysVarBtagJet0);

    }//for(icuts)     
    
  }//for(events)

  // Get elapsed time
  sw.Stop();
  std::cout << "==================================================] 100% " << std::endl;
  std::cout << "--- End of event loop: "; sw.Print();
  

  /***************************
     TTbar Normalizations
  ***************************/

  if(fname.Contains("TT")){
    
    for(int icut=0; icut<4;icut++){
      for(int ich=0; ich<3;ich++){

  	if(fname.Contains("19468.3")){//Temporal Solution
  	  // Xsec(8TeV) NNLO+NNLL + Br correction for the inclusive samples
  	  if(fname.Contains("TTbar_Madgraph") || 
  	     fname.Contains("MadSpin") || 
  	     fname.Contains("TTJetsMGtauolaBkg")) EffEvent[icut][ich] = EffEvent[icut][ich]*(252.0/245.8)*(0.108*9)*(0.108*9); 
  	  // Xsec(8TeV) NNLO+NNLL
  	  else if(fname.Contains("TTJetsFullLeptMGtauola")) EffEvent[icut][ich] = EffEvent[icut][ich]*(252.0/245.8);
  	}
	
    	if(fname.Contains("5100")){//Temporal Solution
	  if(fname.Contains("TTbar_Madgraph") || 
	     fname.Contains("MadSpin") || 
	     fname.Contains("PowhegV2")){
	    // Br correction for the inclusive samples
	    EffEvent[icut][ich] = EffEvent[icut][ich]*(177.3/163.0)*(0.108*9)*(0.108*9); 
	    // SF_pT_reweight mean value 
	    //std::cout << "SF_pT= " << SF_pTweight[icut][ich] << " Cut= "<< icut << " channel= " << ich << std::endl;
	    SF_pTweight[icut][ich]=SF_pTweight[icut][ich]/AccEvent[icut][ich]; 
	    //std::cout << "<SF_pT>= " << SF_pTweight[icut][ich] << std::endl;
	    
	    // pT Reweight: Only for Systematic Uncertainty
	    //EffEvent[icut][ich] = EffEvent[icut][ich]*(1.0/SF_pTweight[icut][ich]); 
	  }
  	}
	
      } // for(icut)
    } // for(ich)
  } // if(TT)


  //Acceptance-Efficiency
  std::cout << "--------  Acceptace  --------" << std::endl;
  std::cout << "Number of RAW-mumu events:" << std::endl;
  std::cout << "Dilepton: " << AccEvent[0][0] << std::endl;
  std::cout << "2 Jets: "   << AccEvent[1][0] << std::endl;
  std::cout << "MET: "      << AccEvent[2][0] << std::endl;
  std::cout << "1 btag: "   << AccEvent[3][0] << std::endl;

  std::cout << "--------  Efficiency  --------" << std::endl;
  std::cout << "Number of Weigthed-mumu events:" << std::endl;
  std::cout << "Dilepton: " << EffEvent[0][0] << " +/- " << sqrt(AccEvent[0][0])*EffEvent[0][0]/AccEvent[0][0] << std::endl;
  std::cout << "2 Jets: "   << EffEvent[1][0] << " +/- " << sqrt(AccEvent[1][0])*EffEvent[1][0]/AccEvent[1][0] << std::endl;
  std::cout << "MET: "      << EffEvent[2][0] << " +/- " << sqrt(AccEvent[2][0])*EffEvent[2][0]/AccEvent[2][0] << std::endl;
  std::cout << "1 btag: "   << EffEvent[3][0] << " +/- " << sqrt(AccEvent[3][0])*EffEvent[3][0]/AccEvent[3][0] << std::endl;
  
  std::cout << "--------  Acceptace  --------" << std::endl;
  std::cout << "Number of RAW-ee events:" << std::endl;
  std::cout << "Dilepton: " << AccEvent[0][1] << std::endl;
  std::cout << "2 Jets: "   << AccEvent[1][1] << std::endl;
  std::cout << "MET: "      << AccEvent[2][1] << std::endl;
  std::cout << "1 btag: "   << AccEvent[3][1] << std::endl;

  std::cout << "--------  Efficiency  --------" << std::endl;
  std::cout << "Number of Weigthed-ee events: " << std::endl;
  std::cout << "Dilepton: " << EffEvent[0][1] << " +/- " << sqrt(AccEvent[0][1])*EffEvent[0][1]/AccEvent[0][1] << std::endl;
  std::cout << "2 Jets: "   << EffEvent[1][1] << " +/- " << sqrt(AccEvent[1][1])*EffEvent[1][1]/AccEvent[1][1] << std::endl;
  std::cout << "MET: "      << EffEvent[2][1] << " +/- " << sqrt(AccEvent[2][1])*EffEvent[2][1]/AccEvent[2][1] << std::endl;
  std::cout << "1 btag: "   << EffEvent[3][1] << " +/- " << sqrt(AccEvent[3][1])*EffEvent[3][1]/AccEvent[3][1] << std::endl;
  
  std::cout << "--------  Acceptace  --------" << std::endl;
  std::cout << "Number of RAW-mue events:" << std::endl;
  std::cout << "Dilepton: " << AccEvent[0][2] << std::endl;
  std::cout << "2 Jets: "   << AccEvent[1][2] << std::endl;
  std::cout << "MET: "      << AccEvent[2][2] << std::endl;
  std::cout << "1 btag: "   << AccEvent[3][2] << std::endl;

  std::cout << "--------  Efficiency  --------" << std::endl;
  std::cout << "Number of Weighted-mue events:" << std::endl;
  std::cout << "Dilepton: " << EffEvent[0][2] << " +/- " << sqrt(AccEvent[0][2])*EffEvent[0][2]/AccEvent[0][2] << std::endl;
  std::cout << "2 Jets: "   << EffEvent[1][2] << " +/- " << sqrt(AccEvent[1][2])*EffEvent[1][2]/AccEvent[1][2] << std::endl;
  std::cout << "MET: "      << EffEvent[2][2] << " +/- " << sqrt(AccEvent[2][2])*EffEvent[2][2]/AccEvent[2][2] << std::endl;
  std::cout << "1 btag: "   << EffEvent[3][2] << " +/- " << sqrt(AccEvent[3][2])*EffEvent[3][2]/AccEvent[3][2] << std::endl;


  //Output Dir
  TString dirname="TopResults";   
  // make a dir if it does not exist!!
  struct stat st;
  if(stat(dirname,&st) != 0) system("mkdir " + dirname);
  
  TString systname="";
  bool matchname=false;
  
  TString temp_fname=fname + ".root";
  for(int i=0;i<temp_fname.Sizeof();i++){
    if (i>2){
      if (temp_fname[i-3]=='-' && 
	  temp_fname[i-2]=='1' && 
	  temp_fname[i-1]=='_') matchname=true;
    }
    if (temp_fname[i]=='.') matchname=false;
    if (matchname) systname.Append(temp_fname[i]);
  }
  
  if(!_syst && !systname.Contains("Up") && !systname.Contains("Down") && !systname.Contains("Nom")){
    // Yields
    TString Yieldfile=dirname + "/Yields" + hname + ".h";
    FILE* fyields = fopen(Yieldfile, "a");
    
    fprintf(fyields,"// %s Sample on %s \n", (fname + ".root").Data() , currentDateTime().Data());
    fprintf(fyields,"// %s version \n", hname.Data());
    fprintf(fyields,"float  %s[3][4];//[channel][Cut] \n", systname.Data());
    fprintf(fyields,"float  err_%s[3][4]; //[channel][Cut] \n", systname.Data());
    fprintf(fyields,"// Channel: [0]=mumu [1]=ee [2]=mue \n");
    fprintf(fyields,"// Cut [0]=Dielpton [1]= Jets+Zveto [2]=MET [3]=btag \n");
    for(int ch=0;ch<3;ch++){
      for(int cut=0;cut<4;cut++){
      fprintf(fyields,"%s[%i][%i] = %.3f ; \n", systname.Data(), ch, cut, EffEvent[cut][ch]);
      if(AccEvent[cut][ch]!=0.0) fprintf(fyields,"err_%s[%i][%i] = %.3f ; \n", systname.Data(), ch, cut, sqrt(AccEvent[cut][ch])*EffEvent[cut][ch]/AccEvent[cut][ch]);
      else fprintf(fyields,"err_%s[%i][%i] = 0.0 ; \n", systname.Data(), ch, cut);
      }
    }
    fclose(fyields);
    
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    std::cout << "Yields saved into " << Yieldfile << " file" << std::endl;
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;    
  }
  
  if (_syst){    
    
    if(!systname.Contains("Up") && 
       !systname.Contains("Down") && 
       !systname.Contains("Nom") &&
       !_tr_unc &&
       !_idiso_unc){
      systname = systname + "_Central";
      fname    = fname    + "_Central";    
    }
    // Trigger and ID/ISO Uncertainty
    
    // Trigger Uncertainty
    else if(!systname.Contains("Up") && 
	    !systname.Contains("Down") && 
	    !systname.Contains("Nom") &&
	    _tr_unc &&
	    !_idiso_unc){
      
      if(TrUnc=="Up"){
	systname = systname + "_Tr_Up";
	fname    = fname    + "_Tr_Up";
      }
      else if(TrUnc=="Down"){
	systname = systname + "_Tr_Down";
	fname    = fname    + "_Tr_Down";
      }
      else{
	systname = systname + "_Tr_Central";
	fname    = fname    + "_Tr_Central";
      }
    }
    
    // ID/IDO Uncertainty
    else if(!systname.Contains("Up") && 
	    !systname.Contains("Down") && 
	    !systname.Contains("Nom") &&
	    !_tr_unc &&
	    _idiso_unc) {
      
      if(IDISOUnc=="Up"){
	systname = systname + "_IDISO_Up";
	fname    = fname    + "_IDISO_Up";
      }
      else if(IDISOUnc=="Down"){
	systname = systname + "_IDISO_Down";
	fname    = fname    + "_IDISO_Down";
      }
      else{
	systname = systname + "_IDISO_Central";
	fname    = fname    + "_IDISO_Central";
      }
    }

    // Systematic Uncertainty Estimations
    TString Syst_Yieldfile=dirname + "/Syst_Yields" + hname + ".h";
    FILE* fSys = fopen(Syst_Yieldfile, "a");        
    
    fprintf(fSys,"// %s Sample on %s \n", (fname + ".root").Data(), currentDateTime().Data());
    fprintf(fSys,"// %s version \n", hname.Data());
    fprintf(fSys,"float  %s[3][4];//[channel][Cut] \n", systname.Data());
    fprintf(fSys,"float  err_%s[3][4]; //[channel][Cut] \n", systname.Data());
    fprintf(fSys,"// Channel: [0]=mumu [1]=ee [2]=mue \n");
    fprintf(fSys,"// Cut [0]=Dielpton [1]= Jets+Zveto [2]=MET [3]=btag \n");
    for(int ch=0;ch<3;ch++){
      for(int cut=0;cut<4;cut++){
	fprintf(fSys,"%s[%i][%i] = %.3f ; \n", systname.Data(), ch, cut, EffEvent[cut][ch]);
	if(AccEvent[cut][ch]!=0.0) fprintf(fSys,"err_%s[%i][%i] = %.3f ; \n", systname.Data(), ch, cut, sqrt(AccEvent[cut][ch])*EffEvent[cut][ch]/AccEvent[cut][ch]);
	else fprintf(fSys,"err_%s[%i][%i] = 0.0 ; \n", systname.Data(), ch, cut);

      }
    }
    fclose(fSys);
    
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    std::cout << "Yields saved for Syst. estimation into " << Syst_Yieldfile << " file" << std::endl;
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;    
    
  }
  
  
  // --- Write histograms
  
  TString outfname=dirname + "/hSF-" + hname + "_" + fname + ".root";
  TFile *target  = new TFile(outfname,"RECREATE" );  

  for(int j=0; j<4; j++){
    for(int i=0; i<3; i++){
      
      hPV[j][i]->Write();
    
      hMET[j][i]->Write();
      hMET_Phi[j][i]->Write();
      hMETSig[j][i]->Write();
      
      hInvMass[j][i]->Write();
      hDiLepPt[j][i]->Write();
      hLep0Pt[j][i]->Write();
      hLep1Pt[j][i]->Write();
      hLep0Eta[j][i]->Write();
      hLep1Eta[j][i]->Write();
      hDelLepPhi[j][i]->Write();
      hLep0Phi[j][i]->Write();
      hLep1Phi[j][i]->Write();

      hNJets[j][i]->Write();
      hHT[j][i]->Write();
      hNBtagJets5[j][i]->Write();
      hNBtagJets[j][i]->Write();
      hJet0Pt[j][i]->Write();    
      hJet1Pt[j][i]->Write();    
      hBtagJet0Pt[j][i]->Write();    
            
      hDYIn[j][i]->Write();
      hDYOut[j][i]->Write();
      hDYIn_btag0[j][i]->Write();
      hDYOut_btag0[j][i]->Write();
      hDYIn_btag1[j][i]->Write();
      hDYOut_btag1[j][i]->Write();
      hDY_NPV[j][i]->Write();
      hDY_NJets[j][i]->Write();
      hDY_TF_InvMass[j][i]->Write();

      hSFpT[j][i]->Write();
      hSFpTError[j][i]->Write();

      hSFIDISO[j][i]->Write();
      hSFIDISOError[j][i]->Write();
      hSFTrigger[j][i]->Write();
      hSFTriggerError[j][i]->Write();

      hUncJet0[j][i]->Write();
      hUncJet1[j][i]->Write();
      hUncBtagJet0[j][i]->Write();

      hSysVarJet0[j][i]->Write();
      hSysVarJet1[j][i]->Write();
      hSysVarBtagJet0[j][i]->Write();

      hUncJet0Pt[j][i]->Write();
      hUncJet1Pt[j][i]->Write();
      hUncBtagJet0Pt[j][i]->Write();

      hSysVarJet0Pt[j][i]->Write();
      hSysVarJet1Pt[j][i]->Write();
      hSysVarBtagJet0Pt[j][i]->Write();
      
    }//for(i)

    hSFIDISO_mu[j]->Write();
    hSFIDISO_e[j]->Write();
    hSFIDISOError_mu[j]->Write();
    hSFIDISOError_e[j]->Write();

    h2DSFIDISO_mu[j]->Write();
    h2DSFIDISO_e[j]->Write();

  }//for(j)
  
  std::cout << "File saved as " << outfname << std::endl;

}


#endif

