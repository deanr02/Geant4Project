// ROOT macro file for plotting example B4 ntuple
//
// Can be run from ROOT session:
// root[0] .x plotNtuple.C


TGraphErrors* create_graph_from_histos(double r, double t) {
    const int n = 5;
    double x[n]  = {1, 10, 100, 1000, 10000};
    double y[n];      // to hold means
    double yerr[n];   // to hold std deviations
    double xerr[n] = {0};  // no x error

    TString pathname = Form("data/%.1f_%.1f/", r, t);

    // Base filenames (no path)
    TString base_filenames[n] = {
        "1MeV.root",
        "10MeV.root",
        "100MeV.root",
        "1000MeV.root",
        "10000MeV.root"
    };

    TString histname = "E_LAr_h1";  // name of histogram in all files

    for (int i = 0; i < n; ++i) {
        TString fullpath = pathname + base_filenames[i];

        TFile* f = TFile::Open(fullpath);
        if (!f || f->IsZombie()) {
            Error("create_graph_from_histos", "Cannot open file: %s", fullpath.Data());
            return;
        }

        TH1* h = dynamic_cast<TH1*>(f->Get(histname));
        if (!h) {
            Error("create_graph_from_histos", "Histogram '%s' not found in file %s",
                  histname.Data(), fullpath.Data());
            f->Close();
            return;
        }

        y[i]    = h->GetMean();
        yerr[i] = h->GetStdDev();

        f->Close();
    }

    // Create TGraphErrors
    TGraphErrors* graph = new TGraphErrors(n, x, y, xerr, yerr);
    graph->SetTitle(Form("r=%.1f, t=%.1f", r, t));
    graph->SetMarkerStyle(21);
    graph->SetLineWidth(2);
    return graph;
}

void plot_all_graphs() {
    std::vector<double> r_vals = {0.05, 0.25, 0.5, 0.75, 0.95};
    std::vector<double> t_vals = {10,50,100,500};

    TCanvas* c = new TCanvas("c", "All Graphs", 900, 700);
    c->SetLogx();

    TLegend* legend = new TLegend(0.6, 0.7, 0.88, 0.88);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    legend->SetTextSize(0.035);

    int color_idx = 1;
    bool first_graph = true;

    for (double r : r_vals) {
        for (double t : t_vals) {
            TGraphErrors* gr = create_graph_from_histos(r, t);
            if (!gr) continue;

            gr->SetLineColor(color_idx);
            gr->SetMarkerColor(color_idx);
            gr->SetMarkerStyle(20 + color_idx % 10);
            gr->SetLineWidth(2);

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
    c->Update();
    c->SaveAs("analysis/all_graphs.png");
}

{
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  plot_all_graphs();
}