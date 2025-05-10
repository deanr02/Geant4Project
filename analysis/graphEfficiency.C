// ROOT macro to generate and plot TGraphErrors for combinations of r and t
// Each histogram is stored in path: "<r>_<t>/hist_*.root"
// Each histogram has name: "myHist"
// The graph is saved as PNG image: "all_graphs.png"

{
  gROOT->Reset();
  gROOT->SetStyle("Plain");

  const int nr = 2;
  const int nt = 2;

  double r_vals[nr] = {1.0, 2.0};
  double t_vals[nt] = {5.0, 10.0};

  TCanvas* c = new TCanvas("c", "All Graphs", 900, 700);
  c->SetLogx();

  TLegend* legend = new TLegend(0.6, 0.7, 0.88, 0.88);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);
  legend->SetTextSize(0.035);

  int color_idx = 1;
  bool first_graph = true;

  for (int i = 0; i < nr; i++) {
    for (int j = 0; j < nt; j++) {
      double r = r_vals[i];
      double t = t_vals[j];

      const int n = 5;
      double x[n]  = {1, 10, 100, 1000, 10000};
      double y[n];
      double yerr[n];
      double xerr[n] = {0, 0, 0, 0, 0};

      TString pathname = Form("data/%.1f_%.1f/", r, t);
      TString base_filenames[n] = {
        "1MeV.root", "10MeV.root", "100MeV.root",
        "1000MeV.root", "10000MeV.root"
      };
      TString histname = "myHist";

      bool valid = true;
      for (int k = 0; k < n; k++) {
        TString fullpath = pathname + base_filenames[k];
        TFile* f = TFile::Open(fullpath);
        if (!f || f->IsZombie()) {
          std::cerr << "Cannot open file: " << fullpath << std::endl;
          valid = false;
          break;
        }

        TH1* h = (TH1*)f->Get(histname);
        if (!h) {
          std::cerr << "Histogram not found in file: " << fullpath << std::endl;
          f->Close();
          valid = false;
          break;
        }

        y[k]    = h->GetMean();
        yerr[k] = h->GetStdDev();
        f->Close();
      }

      if (!valid) continue;

      TGraphErrors* gr = new TGraphErrors(n, x, y, xerr, yerr);
      gr->SetLineColor(color_idx);
      gr->SetMarkerColor(color_idx);
      gr->SetLineWidth(2);
      gr->SetMarkerStyle(20 + color_idx % 10);

      if (first_graph) {
        gr->Draw("APL");
        first_graph = false;
      } else {
        gr->Draw("PL SAME");
      }

      legend->AddEntry(gr, Form("r=%.1f, t=%.1f", r, t), "l");
      color_idx++;
    }
  }

  legend->Draw();
  c->SaveAs("analysis/all_graphs.png");
}
