// ROOT macro to generate and plot TGraphErrors for combinations of r and t
// Each histogram is stored in path: "<r>_<t>/hist_*.root"
// Each histogram has name: "myHist"
// The graph is saved as PNG image: "all_graphs.png"

{
  gROOT->Reset();
  gROOT->SetStyle("Plain");


  const int nr = 5;
  const int nt = 4;

  TString r_vals[nr] = {"0.05", "0.25", "0.5", "0.75", "0.95"};


  TString t_vals[4] = {"10","50","100","500"};
  //TString t = "100";
  TString E_vals[4] = {"10", "100", "1000", "10000"};
  //TString t = "100";

  gStyle->SetTitleFontSize(15); // Set the title size for all axis
  gStyle->SetTitleX(0.076); // Position the title X-coordinate to 0.1
  gStyle->SetTitleY(0.99); // Position the title Y-coordinate to 0.95
  gStyle->SetTitleW(0.85); // Set the title width to 0.5
  gStyle->SetTitleH(0.05); // Set the title height to 0.1

 for (int j = 0; j < 4; j++) {
   TString t = t_vals[j];
   //TString t = "10";
    std::cout << "testing t " + t << std::endl;

    TString title =  "Calorimeter Efficiency vs Electron Energy for " + t + " mm Layer Geometries";

    TCanvas* c = new TCanvas("c", title, 900, 700);
 
    // c->SetTitle( E + " MeV Electron Shower grofiles for " + t + " mm Layer Geometries");
    TLegend* legend = new TLegend(0.55, 0.65, 0.75, 0.85 );
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    legend->SetTextSize(0.035);

    int color_idx = 1;
    bool first_graph = true;

    for (int i = 0; i < nr; i++) {

      TString r = r_vals[i];
      //TString t = t_vals[j];
      //TString E = E_vals[j];
      std::cout << "testing r " + r << std::endl;

      const int n = 5;
      double x[n]  = {1, 10, 100, 1000, 10000};
      double y[n];
      double yerr[n];
      double xerr[n] = {0, 0, 0, 0, 0};
      int zero_count = 0;

      TString pathname = Form("data/" + r + "_" + t + "/");
      TString base_filenames[n] = {
        "1MeV.root", "10MeV.root", "100MeV.root",
        "1000MeV.root", "10000MeV.root"
      };
      TString histname = "histograms/E_LAr_h1";

      bool valid = true;
      for (int k = 0; k + zero_count < n; k++) {


        TString fullpath = pathname + base_filenames[k];

        std::cout << "going through file " + fullpath << std::endl;

        TFile* f = TFile::Open(fullpath);
        if (!f || f->IsZombie()) {
          std::cerr << "Cannot open file: " << fullpath << std::endl;
          valid = false;
          break;
        }

        TH1D* h = (TH1D*)f->Get(histname);
        if (!h) {
          std::cerr << "Histogram not found in file: " << fullpath << std::endl;
          f->Close();
          valid = false;
          break;
        }

        
            y[k]    = (h->GetMean())/x[k];
            yerr[k] = (h->GetStdDev())/x[k];
        
    

        std::cout << "Mean E " + std::to_string(y[k]) << std::endl;
        f->Close();

        if (!valid) continue;


 
          

      }
      

      std::cout<< "n " + std::to_string(n) << std::endl;
        std::cout<< "x[0]" + std::to_string(x[0]) << std::endl;
        std::cout<< "y[0]" + std::to_string(y[0]) << std::endl;



      if (color_idx == 5){
        color_idx++;
      }

      std::cout << "color " + std::to_string(color_idx) << std::endl;
      TGraphErrors* gr = new TGraphErrors(n, x, y, xerr, yerr);
      gr->SetLineColor(color_idx);
      gr->SetMarkerColor(color_idx);
      gr->SetLineWidth(2);
      gr->SetMarkerStyle(20 + color_idx % 10);
      gr->GetXaxis()->SetTitle("Incident Electron Energy (MeV)");
      gr->GetYaxis()->SetTitle("Efficiency (E_{detected} / E_{incident})");
      gr->GetXaxis()->CenterTitle();
      gr->GetYaxis()->CenterTitle();
      gr->GetXaxis()->SetRangeUser(0,10000);
      gr->GetYaxis()->SetRangeUser(0,1);


  
      gr->GetXaxis()->SetTitleOffset(1.4);
      gr->GetYaxis()->SetTitleOffset(1.4);
      gr->SetTitle(title);
      gPad->SetLogx();
            gPad->SetLogy();


      gPad->SetLeftMargin(0.14);
      gPad->SetBottomMargin(0.14);
      gr->GetYaxis()->SetRangeUser(0.0001,1);

      if (first_graph) {
        gr->Draw();
        first_graph = false;
      std::cout<< "drawing graph " << std::endl;

      } else {
        gr->Draw("SAME");
              std::cout<< "drawing graph again" << std::endl;

      }
    
      // gStyle->SetTitleFontSize(2);


     // gr->SetStats(0);

      legend->AddEntry(gr, Form("r=" + r), "l");
      color_idx++;
    
      
    }
          std::cout<< "drawing canvas " << std::endl;

    legend->SetHeader("LAr Ratio (z_{LAr}/z_{Pb})");
    legend->Draw();
    c->Update();
gSystem->ProcessEvents();

    c->SaveAs("analysis/eff_" + t + "mm.png");
        c->SaveAs("images/eff_" + t + "mm.png");

    std::cout<< "saving analysis/eff_" + t + "mm.png" << std::endl;

  }
}
