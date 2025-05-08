//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B4/B4d/src/RunAction.cc
/// \brief Implementation of the B4::RunAction class

#include "RunAction.hh"


#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

extern G4int nofLayers;

namespace B4
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);

  // Create analysis manager
  // The choice of the output format is done via the specified
  // file extension.
  auto analysisManager = G4AnalysisManager::Instance();
  // Create directories
  // analysisManager->SetHistoDirectoryName("histograms");
  // analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
  // Note: merging ntuples is available only with Root output

  // Book histograms, ntuple
  //
  G4int nLayers = nofLayers;
  std::cout << nLayers << std::endl;
  // Creating histograms
  analysisManager->CreateH2("Eabs", "Edep in absorber", nLayers, 0, nLayers, 110, 0., 330 * MeV);
  analysisManager->CreateH2("Egap", "Edep in gap", nLayers, 0, nLayers, 100, 0., 30 * MeV);
  analysisManager->CreateH2("NCabs", "NChargeTracks in absorber", nLayers, 0, nLayers, 10, 0., 50);
  analysisManager->CreateH2("NCgap", "NChargeTracks in gap", nLayers, 0, nLayers, 10, 0., 50);

  // Creating ntuple
  //
  // analysisManager->CreateNtuple("B4", "Edep and TrackL");
  // analysisManager->CreateNtupleDColumn("Eabs");
  // analysisManager->CreateNtupleDColumn("Egap");
  // analysisManager->CreateNtupleDColumn("NCabs");
  // analysisManager->CreateNtupleDColumn("NCgap");
  // analysisManager->CreateNtupleDColumn("NPabs");
  // analysisManager->CreateNtupleDColumn("NPgap");
  // analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  // inform the runManager to save random number seed
  // G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = "B4.root";
  // Other supported output types:
  // G4String fileName = "B4.csv";
  // G4String fileName = "B4.hdf5";
  // G4String fileName = "B4.xml";
  analysisManager->OpenFile(fileName);
  G4cout << "Using " << analysisManager->GetType() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // print histogram statistics
  //
  auto analysisManager = G4AnalysisManager::Instance();

  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B4
