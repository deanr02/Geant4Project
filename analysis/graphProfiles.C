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
  TString t_vals[nt] = {"10","50","100","500"};

  TCanvas* c = new TCanvas("c", "Calorimeter Efficiency vs Electron Energy for Various Layer Geometries", 900, 700);
  c->SetLogx();
  c->SetLogy();
  c->SetTitle("Calorimeter Efficiency vs Electron Energy for Various Layer Geometries");



  TLegend* legend = new TLegend(0.6, 0.7, 0.88, 0.88);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);
  legend->SetTextSize(0.035);

  int color_idx = 1;
  bool first_graph = true;

  for (int i = 0; i < nr; i++) {
    for (int j = 0; j < nt; j++) {
      TString r = r_vals[i];
      TString t = t_vals[j];

      const int n = 5;
      double x[n]  = {1, 10, 100, 1000, 10000};
      double y[n];
      double yerr[n];
      double xerr[n] = {0, 0, 0, 0, 0};

      TString pathname = Form("data/" + r + "_" + t + "/");
      TString base_filenames[n] = {
        "1MeV.root", "10MeV.root", "100MeV.root",
        "1000MeV.root", "10000MeV.root"
      };
      TString histname = "histograms/E_LAr_h1";

      bool valid = true;
      for (int k = 0; k < n; k++) {
        TString fullpath = pathname + base_filenames[k];
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
        f->Close();
      }

      if (!valid) continue;

      TGraphErrors* gr = new TGraphErrors(n, x, y, xerr, yerr);
      gr->SetLineColor(color_idx);
      gr->SetMarkerColor(color_idx);
      gr->SetLineWidth(2);
      gr->SetMarkerStyle(20 + color_idx % 10);
      gr->GetXaxis()->SetTitle("Incident Electron Energy (MeV)");
      gr->GetYaxis()->SetTitle("Calorimeter Efficiency (E_{det} / E_{inc})");
      gr->SetTitle("Calorimeter Efficiency vs Electron Energy for Various Layer Geometries");
      gr->GetXaxis()->CenterTitle();
      gr->GetYaxis()->CenterTitle();
      //gr->GetHistogram()->SetTitleSize(0.1);
 

      gPad->SetLeftMargin(0.14);
      gPad->SetBottomMargin(0.14);
      gPad->SetTitle("Calorimeter Efficiency vs Electron Energy for Various Layer Geometries");

      gr->GetXaxis()->SetTitleOffset(1.4);
      gr->GetYaxis()->SetTitleOffset(1.4);

      gr->SetMaximum(1);
      gr->SetMinimum(0.0001);

      if (first_graph) {
        gr->Draw("APL");
        first_graph = false;
      } else {
        gr->Draw("PL SAME");
      }

      legend->AddEntry(gr, Form("r=" + r +" , t=" + t), "l");
      color_idx++;
    }
  }

  legend->Draw();
  c->SaveAs("analysis/efficiency.png");
}
