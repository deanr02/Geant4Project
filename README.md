# PHYS 4410 Skill Project: Geant4

* Work in progress

## Calorimeter Simulation Source Code

* Built from Geant4 basic example B4
* Simualtes LAr Pb calorimeter
* `exampleB4d.cc`
  * Geometry parameter inputs r and t added
  * Detector volume variables calculated from inputs
* `src\`
  * `EventAction.cc`, `DetectorConstruction.cc`, and `RunAction.cc` modified

## Run Scripts

* `runEnergy.mac`
   * Run 1000 events at a single electron energy
* `runAllEnergies.mac`
  * Run 1000 events at electron energies 1, 10, 100, 1000, 10000 MeV
* `runAllGeometries.ps1`
  * Creates simulations with varying LAr/Pb ratios and layer thicknesses, and calls runAllEnergies.mac
  * Saves output files to `data\`

## Data

* Folders titled `r_t\` where
  
    $r = \frac{z_{LAr}}{z_{Pb}}$ and $t = z_{LAr} + z_{Pb}$.
  
* ROOT Structure
  * `histograms\` Constants all histograms and profiles
  * `ntuples\` contains ntuples
* 1D Histograms
  * `E_LAr_h1`: Total energy deposited in LAr (MeV)
  * `E_Pb_h1`: Total energy deposited in Pb (Mev)
* 2D Histograms
  * `E_Lar_h2`: Position of layer (mm), Energy deposited in LAr layer (Mev)
  * `E_Pb_h2`: Position of layer (mm), Energy deposited in Pb layer (Mev)
* 1D Profiles
  * `E_Lar_p1`: Avg energy deposited in LAr layer (Mev) vs Position of layer (mm)
  * `E_Pb_p1`: Avg energy deposited in Pb layer (Mev) Position of layer (mm)
* Ntuple `ntuple` branches
  * `E_dep_Pb` Total energy deposited in Pb (MeV)
  * `E_dep_LAr` Total energy deposited in LAr (MeV)

## Analysis

* `\analysis\graphEfficiency.C` ROOT macro to collect mean total energy deposited in LAr for each electron energy, and plot caloriemter efficiency vs electron energy with error bars = standard deviation. Plots graphs for every layer geometry
  * calorimeter efficiency defined as:
  
     $\epsilon = \frac{\text{Total E deposited in LAr}}{\text{Incident electron E}}$
