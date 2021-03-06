/// \file RunAction.cc
/// \brief Implementation of the RunAction class
#include <sstream>
#include "DetectorConstruction.hh"
#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "NtupleBuffsize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
 : G4UserRunAction()
{ 
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in Analysis.hh
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;
  Nbuff = NTUPLEBUFFSIZE;    // defined in "NtupleBuffsize.hh"
  // Create directories 
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output

  // Book histograms, ntuple
  //
  // Creating histograms

  if(1) analysisManager->CreateH1("TDepl", "Time Edep in Depletion;T(ns);E(keV)", 300, 0.0, 10000.0);  // NTUPLE_TIMEDEPL   ==  0 
  if(0) analysisManager->CreateH1("ECmos", "Edep in CMOS layer;E(keV);N"        , 200, 0.0,1000.0);
  if(0) analysisManager->CreateH1("EDepl", "Edep in depletion layer;E(KeV);N"   , 200, 0.0,1000.0);
  if(0) analysisManager->CreateH1("EWafer","Edep in wafer;E(keV);N"             , 200, 0.0,5000.0);
  if(0) analysisManager->CreateH1("LCmos", "Track Length in CMOS;L(um);N"       , 100, 0.0, 600.0);
  if(0) analysisManager->CreateH1("LDepl", "Track Length in Depletion;L(um);N"  , 100, 0.0, 600.0);
  if(0) analysisManager->CreateH1("LWafer","Track Length in Wafer;L(um);N"      , 100, 0.0, 600.0);
  if(0) analysisManager->CreateH1("TCmos", "Time Edep in CMOS;T(ns);E(keV)"     , 300, 0.0, 10000.0);
  if(0) analysisManager->CreateH1("TWafer","Time of Edep in Wafer;T(ns);E(keV)" , 300, 0.0, 10000.0);

  // Creating ntuple
  //
  analysisManager->CreateNtuple("B4", "EventData");
  analysisManager->CreateNtupleIColumn("NPixXY");    //  0
  analysisManager->CreateNtupleIColumn("NBuff");     //  1
  analysisManager->CreateNtupleDColumn("BeamPosX0"); //  2
  analysisManager->CreateNtupleDColumn("BeamPosY0"); //  3
  analysisManager->CreateNtupleDColumn("BeamPosZ0"); //  4
  analysisManager->CreateNtupleDColumn("BeamMomX0"); //  5
  analysisManager->CreateNtupleDColumn("BeamMomY0"); //  6
  analysisManager->CreateNtupleDColumn("BeamMomZ0"); //  7
  analysisManager->CreateNtupleIColumn("Nhits");     //  8
  analysisManager->CreateNtupleIColumn("DUMMY09");     //  9
  analysisManager->CreateNtupleIColumn("DUMMY10");     //  10
  analysisManager->CreateNtupleIColumn("DUMMY11");     //  11
  analysisManager->CreateNtupleIColumn("DUMMY12");     //  12
  analysisManager->CreateNtupleIColumn("DUMMY13");     //  13
  analysisManager->CreateNtupleIColumn("DUMMY14");     //  14
  analysisManager->CreateNtupleIColumn("DUMMY15");     //  15
  analysisManager->CreateNtupleIColumn("DUMMY16");     //  16
  analysisManager->CreateNtupleIColumn("DUMMY17");     //  17
  analysisManager->CreateNtupleIColumn("DUMMY18");     //  18
  analysisManager->CreateNtupleIColumn("DUMMY19");     //  19


  //  analysisManager->CreateNtupleDColumn("PixelData");
  std::stringstream ss;
  for (G4int i=0; i<Nbuff; i++) {
    ss.str("");  ss<< "IADR" << i;             //20,23,26,....
    analysisManager->CreateNtupleIColumn(ss.str().c_str());
    ss.str("");  ss<< "E" << i;                //21,24,27,....
    analysisManager->CreateNtupleDColumn(ss.str().c_str());
    //    ss.str("");  ss<< "Charge" << i;           //22,25,28,....
    //    analysisManager->CreateNtupleDColumn(ss.str().c_str());
  }
  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = "B4_2MeV";
  analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  auto analysisManager = G4AnalysisManager::Instance();
#if 0
  // print statistics
  if ( analysisManager->GetH1(1) ) {
    G4cout << G4endl << " ----> print histograms statistic ";
    if(isMaster) {
      G4cout << "for the entire run " << G4endl << G4endl; 
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl; 
    }
    
    G4cout << " Esoi : mean = " 
       << G4BestUnit(analysisManager->GetH1(0)->mean(), "Energy") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(0)->rms(),  "Energy") << G4endl;
    
    G4cout << " Edepl : mean = " 
       << G4BestUnit(analysisManager->GetH1(2)->mean(), "Energy") 
       << " rms = "
       << G4BestUnit(analysisManager->GetH1(2)->rms(),  "Energy") << G4endl;
    
    G4cout << " Ewaf : mean = " 
       << G4BestUnit(analysisManager->GetH1(4)->mean(), "Energy") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(4)->rms(),  "Energy") << G4endl;

  }
#endif
  // save histograms & ntuple
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
