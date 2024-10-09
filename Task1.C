#include <iostream>
#include "TFile.h"
#include "TTree.h"

int Task1() {

    const char* filePath = "/eos/project-h/h8ctb/2024_H8_TestBeams/H8TB_202405_PPSTiming_Diamond_QFib_psQ_Parasitic/PSQ/Runs/Reco_Runs/Run3.root";

    TFile* file = TFile::Open(filePath);

    TTree* tree = (TTree*)file->Get("LeCroyTree");

    if (tree) {
        std::cout << "Tree loaded successfully!" << std::endl;
    } else {
        std::cout << "Failed to load tree from file: " << filePath << std::endl;
    }
    
    std::vector<Double_t> AmplSamples;  // Vector for amplitudes
    std::vector<Double_t>* pAmplSamples = &AmplSamples;  // Pointer to amplitudes

    Double_t SamplingPeriod = 0.0;  // Sampling period
    Double_t data_Hoffset = 0.0;    // Time offset
    
    tree->SetBranchAddress("AmplSamples", &pAmplSamples);
    tree->SetBranchAddress("SamplingPeriod", &SamplingPeriod);
    tree->SetBranchAddress("data_Hoffset", &data_Hoffset);
    
    TCanvas* canvas = new TCanvas("canvas", "Trigger 1", 900, 600);
    canvas->Divide(2, 2); 
    for (int trigger = 0; trigger < 40; ++trigger) {

        TCanvas* canvas = new TCanvas(Form("canvas_trigger_%d", trigger), Form("Trigger %d", trigger + 1), 1200, 800);
        canvas->Divide(2, 2);  
        
        // Zwiększ margines górny, aby zrobić miejsce na tytuł
        canvas->SetTopMargin(0.15);  // Zwiększenie marginesu górnego

        // Ustaw tytuł wyżej
        TLatex latex;
        latex.SetTextSize(0.03);
        latex.SetTextFont(42);
        latex.DrawLatexNDC(0.45, 0.98, Form("Trigger %d", trigger + 1));  // Umieść tytuł wyżej
        
        canvas->SetLeftMargin(0.0);
        canvas->SetRightMargin(0.0);
        canvas->SetBottomMargin(0.1);

        for (int entry = 0; entry < 4; ++entry) {
            tree->GetEntry(entry + trigger * 4);  

            int nSamples = AmplSamples.size();

            std::vector<double> time(nSamples);
            for (int i = 0; i < nSamples; ++i) {
                time[i] = data_Hoffset + i * SamplingPeriod;  
            }

            TGraph* graph = new TGraph(nSamples, &time[0], &(*pAmplSamples)[0]);
            graph->SetTitle(Form("Channel %d", entry));
            graph->GetXaxis()->SetTitle("Time (s)");
            graph->GetYaxis()->SetTitle("Amplitude (V)");

            // Zwiększenie rozmiaru czcionki podpisów podziałek (numery)
            graph->GetXaxis()->SetLabelSize(0.05);  // Większe numery na osi X
            graph->GetYaxis()->SetLabelSize(0.05);
            graph->GetXaxis()->SetTitleSize(0.05);  // Większe numery na osi X
            graph->GetYaxis()->SetTitleSize(0.05);  // Większe numery na osi Y

            graph->SetLineWidth(1);
		
            canvas->cd(entry + 1);
            gPad->SetLeftMargin(0.15);  // Ustawienie większego marginesu po lewej, aby nie ucinać osi Y
            gPad->SetBottomMargin(0.13); // Margines dolny, aby etykiety osi X się mieściły
            graph->Draw("AL");  
        }

        canvas->SaveAs(Form("Plots/WaveForms/Trigger%d.png", trigger + 1));
        
        delete canvas;
    }

    file->Close();

    return 0;
}
