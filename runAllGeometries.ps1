# --- run‑grid.ps1 ----------------------------------------------------------
$fvals = 0.2,0.5,0.8
$tvals = 10,5,10,25,50,75,100,500,1000

foreach ($f in $fvals) {
    foreach ($t in $tvals) {

        # 1) run the program
        .\build4\Release\exampleB4d.exe -m .\build4\runAllEnergies.mac $f $t

        # 2) create a folder like 0.1_1_folder
        $folder = "data\{0}_{1}" -f $f, $t
        if (-not (Test-Path $folder)) {
            New-Item -ItemType Directory -Path $folder | Out-Null
        }

        # 3) move every data*.txt file into that folder
        Get-ChildItem -Path ".\build4\*.root" -File |               # pick matching files
            Move-Item -Destination $folder -Force -ErrorAction SilentlyContinue
            # -Force overwrites if re‑running; -ErrorAction avoids warnings if none found
    }
}
# --------------------------------------------------------------------------