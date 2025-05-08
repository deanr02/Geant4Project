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
/// \file B4/B4d/src/EventAction.cc
/// \brief Implementation of the B4d::EventAction class

#include "EventAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4THitsMap.hh"
#include "G4UnitsTable.hh"
#include "G4VScoreHistFiller.hh"

#include <iomanip>

namespace B4d
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4THitsMap<G4double>* EventAction::GetHitsCollection(G4int hcID, const G4Event* event) const
{
  auto hitsCollection = static_cast<G4THitsMap<G4double>*>(event->GetHCofThisEvent()->GetHC(hcID));

  if (!hitsCollection) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID;
    G4Exception("EventAction::GetHitsCollection()", "MyCode0003", FatalException, msg);
  }

  return hitsCollection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double EventAction::GetSum(G4THitsMap<G4double>* hitsMap) const
{
  G4double sumValue = 0.;
  for (auto it : *hitsMap->GetMap()) {
    // hitsMap->GetMap() returns the map of std::map<G4int, G4double*>
    sumValue += *(it.second);
  }
  return sumValue;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* /*event*/) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  // Get hist collections IDs
  if (fAbsoEdepHCID == -1) {
    fAbsoEdepHCID = G4SDManager::GetSDMpointer()->GetCollectionID("Absorber/Edep");
    fGapEdepHCID = G4SDManager::GetSDMpointer()->GetCollectionID("Gap/Edep");
    fAbsoNChargeTracksHCID = G4SDManager::GetSDMpointer()->GetCollectionID("Absorber/NChargeTracks");
    fGapNChargeTracksHCID = G4SDManager::GetSDMpointer()->GetCollectionID("Gap/NChargeTracks");
  }

  // Get sum values from hits collections
  //
  auto absoEdep = GetHitsCollection(fAbsoEdepHCID, event);
  auto gapEdep = GetHitsCollection(fGapEdepHCID, event);

  auto absoNChargeTracks = GetHitsCollection(fAbsoNChargeTracksHCID, event);
  auto gapNChargeTracks = GetHitsCollection(fGapNChargeTracksHCID, event);

  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // fill histograms
  //
 
  for (auto it : *absoEdep->GetMap()) {
    analysisManager->FillH2(0, it.first, *(it.second));
  }

  for (auto it : *gapEdep->GetMap()) {
    analysisManager->FillH2(1, it.first, *(it.second));
  }
  for (auto it : *absoNChargeTracks->GetMap()) {
    analysisManager->FillH2(2, it.first, *(it.second));
  }
  for (auto it : *gapNChargeTracks->GetMap()) {
    analysisManager->FillH2(3,it.first, *(it.second));
  }


  // fill ntuple
  //
  // analysisManager->FillNtupleDColumn(0, absoEdep->GetMap());
  // analysisManager->FillNtupleDColumn(1, gapEdep->GetMap());
  // analysisManager->FillNtupleDColumn(2, absoNChargeTracks);
  // analysisManager->FillNtupleDColumn(3, gapNChargeTracks);
  // analysisManager->FillNtupleDColumn(4, absoNPhotonTracks);
  // analysisManager->FillNtupleDColumn(5, gapNPhotonTracks);
  // analysisManager->AddNtupleRow();

  // print per event (modulo n)
  //
  auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ((printModulo > 0) && (eventID % printModulo == 0)) {
    G4cout << "--> End of event: " << eventID << "\n" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B4d
