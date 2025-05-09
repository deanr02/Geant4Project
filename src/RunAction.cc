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
#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

extern G4int nofLayers;
extern G4double layerThickness;
extern G4double activeFraction;
extern G4double beamEnergy;
extern G4ParticleGun* gun;

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
  analysisManager->SetHistoDirectoryName("histograms");
  analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
  // Note: merging ntuples is available only with Root output

  // Book histograms, ntuple
  //
  G4double zmax = nofLayers * layerThickness;

  // Creating histograms
  analysisManager->CreateH1("E_Pb_h1", "E_{dep} in Pb" ,  5000, 0., 5000 * MeV);
  analysisManager->CreateH1("E_LAr_h1", "E_{dep} in LAr" ,  5000, 0., 5000 * MeV);
  analysisManager->CreateH2("E_Pb_h2", "E_{dep} in Pb" , nofLayers, 0, zmax, 5000, 0., 5000 * MeV);
  analysisManager->CreateH2("E_LAr_h2", "E_{dep} in LAr", nofLayers, 0, zmax, 5000, 0., 5000 * MeV);
  analysisManager->CreateP1("E_Pb_p1", "E_{dep} in Pb", nofLayers, 0, zmax, 0., 5000 * MeV);
  analysisManager->CreateP1("E_LAr_p1", "E_{dep} in LAr", nofLayers, 0, zmax, 0., 5000 * MeV);
  
  // Creating ntuple
  analysisManager->CreateNtuple("ntuple", "Edep, z, and N_loss");
  analysisManager->CreateNtupleDColumn("Edep_Pb");
  // analysisManager->CreateNtupleDColumn("z_Hit_Pb");
  // analysisManager->CreateNtupleDColumn("Edep_Hit_Pb");
  analysisManager->CreateNtupleDColumn("Edep_LAr");
  // analysisManager->CreateNtupleDColumn("z_Hit_LAr");
  // analysisManager->CreateNtupleDColumn("Edep_Hit_LAr");
  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  // inform the runManager to save random number seed
  // G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  // Get analysis manager

  auto analysisManager = G4AnalysisManager::Instance();

  const auto* base =
  G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction();


  // Open an output file
  //
  G4String fileName = "f_" + std::to_string(activeFraction) + "_t_" + std::to_string(layerThickness) + ".root";
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

  analysisManager->SetH2XAxisTitle(0, "Layer Z Position (mm)");
  analysisManager->SetH2XAxisTitle(1, "Layer Z Position (mm)");
  analysisManager->SetP1XAxisTitle(0, "Layer Z Position (mm)");
  analysisManager->SetP1XAxisTitle(1, "Layer Z Position (mm)");

  analysisManager->SetH2YAxisTitle(0, "E_{dep} (MeV)");
  analysisManager->SetH2YAxisTitle(1, "E_{dep} (MeV)");
  analysisManager->SetP1YAxisTitle(0, "E_{dep} (MeV)");
  analysisManager->SetP1YAxisTitle(1, "E_{dep} (MeV)");


  // save histograms & ntuple
  //

  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B4
