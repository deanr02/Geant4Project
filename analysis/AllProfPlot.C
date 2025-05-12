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

  gStyle->SetTitleFontSize(1000); // Set the title size for all axis
  gStyle->SetTitleX(0.34); // Position the title X-coordinate to 0.1
  gStyle->SetTitleY(0.95); // Position the title Y-coordinate to 0.95
  gStyle->SetTitleW(0.6); // Set the title width to 0.5
  gStyle->SetTitleH(0.18); // Set the title height to 0.1
  gStyle->SetTitle("Title");

  

  TCanvas* c = new TCanvas("c", "canvas", 5000, 5000);

// TLegend* legend = new TLegend(0.55, 0.65, 0.75, 0.85 );
//       legend->SetBorderSize(0);
//       legend->SetFillStyle(0);
//       legend->SetTextSize(0.035);
//                 legend->SetHeader("LAr Ratio (z_{LAr}/z_{Pb})");
//           legend->AddEntry(pr, Form("r=" + r), "l");
//                 legend->Draw();
  c->Divide(4,4);
  int count = 1;

  for (int j = 0; j < 4; j++) {
    TString t = t_vals[j];

    for (int k = 0; k < 4; k++) {

        TPad* pad = (TPad*)c->cd(count);
        pad->SetLeftMargin(0.14);
        pad->SetRightMargin(0.05);
        pad->SetTopMargin(0.025);
        pad->SetBottomMargin(0.1);
  

      count++;

      TString E = E_vals[k];
      
      TString title =  E + " MeV Electron Shower Profiles for " + t + " mm Layer Geometries";


    // c->SetTitle( E + " MeV Electron Shower Profiles for " + t + " mm Layer Geometries");


      int color_idx = 1;
      bool first_graph = true;

      for (int i = 0; i < nr; i++) {

          TString r = r_vals[i];
          //TString t = t_vals[j];
          //TString E = E_vals[j];

          TString pathname = Form("data/" + r + "_" + t + "/" + E + "MeV.root");
          TString profilename = "histograms/E_LAr_p1";
          TString histname = "histograms/E_LAr_h1";

          bool valid = true;
          TFile* f = TFile::Open(pathname);
          if (!f || f->IsZombie()) {
            std::cerr << "Cannot open file: " << pathname << std::endl;
            valid = false;
            break;
          }

          TProfile* prof = (TProfile*)f->Get(profilename);
          TProfile* pr = (TProfile*)prof->Clone();
          prof->SetDirectory(0);
          pr->SetDirectory(0);

          if (!pr) {
            std::cerr << "Profile not found in file: " << profilename << std::endl;
            f->Close();
            valid = false;
            break;
          }

          TH1D* h = (TH1D*)f->Get(histname);
            if (!h) {
              std::cerr << "Histogram not found in file: " << pathname << std::endl;
              f->Close();
              valid = false;
              break;
            }
          double total_E = pr->Integral();

          f->Close();
          
          if (!valid) continue;

          std::cout << total_E << std::endl;
          if (total_E > 0) { 
              pr->Scale(1/total_E);
          } else {
            pr->Scale(0);
          }

          if(color_idx == 5){
            color_idx++;
          }
          
          pr->SetLineColor(color_idx);
          pr->SetLineWidth(2);
          pr->SetMarkerColor(color_idx);
          pr->SetMarkerStyle(20 + color_idx % 10);
          pr->SetTitle("E = " + E + "MeV , t = " + t + " mm");
          pr->SetTitleOffset(0,"Y");
        //  pr->SetTitle( E + " MeV Electron Shower Profiles in LAr Calorimeter for " + t + " mm Layer Geometries");
      
          //gr->GetHistogram()->SetTitleSize(0.1);
    
      

          if (first_graph) {
            pr->Draw("HIST L");
            first_graph = false;

          

          } else {
            pr->Draw("HIST L SAME");
          }

          pr->GetXaxis()->SetTitle("");
         // pr->GetXaxis()->SetTitle("Layer Position (mm)");
            pr->GetXaxis()->CenterTitle();
            pr->GetXaxis()->SetRangeUser(0,1000);
           pr->GetYaxis()->SetRangeUser(0,1);
           pr->GetXaxis()->SetLabelSize(0.07);
            
        pr->GetYaxis()->SetTitle("");
           // pr->GetYaxis()->SetTitle("Energy Deposition (Normalized)");
            pr->GetYaxis()->CenterTitle();
          pr->GetXaxis()->SetTitleOffset(1.3);
            pr->GetYaxis()->SetTitleOffset(1.5);
                pr->GetYaxis()->SetLabelSize(0.07);


           // gStyle->SetTitleFontSize(2);


            pr->SetStats(0);
          color_idx++;
        
      }





    }
  }

        
      c->SaveAs("analysis/allProfilesSameScale.png");
}
