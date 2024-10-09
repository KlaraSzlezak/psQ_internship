#include "TreeReader.h"

TreeReader::TreeReader(const std::string &inputFilePath, const std::string &treeName)
    : theEventCount(0), 
      inputFile(nullptr), 
      tree(nullptr), 
      channelNumber(-1), 
      theConstantFrac(0.1), 
      theFixedThreshold{-0.181267, -0.145013, -0.145013, -0.21752} {

    inputFile = TFile::Open(inputFilePath.c_str(), "READ");

    if (!inputFile || inputFile->IsZombie()) std::cerr << "Failed to open file: " << inputFilePath << std::endl;

    tree = (TTree*)inputFile->Get(treeName.c_str());

    if (tree) {
        std::cout << "Tree loaded successfully!" << std::endl;
    } else {
        std::cerr << "Failed to load tree from file: " << inputFilePath << std::endl;
    }

    tree->SetBranchAddress("AmplSamples"   , &pAmplSamples  );
    tree->SetBranchAddress("SamplingPeriod", &samplingPeriod);
    tree->SetBranchAddress("Channel"       , &channelNumber );
    tree->SetBranchAddress("TrgTime"       , &trgTime       );
    tree->SetBranchAddress("data_Hoffset"  , &hOffset       );

    //std::cout << (pAmplSamples->size()) << std::endl;
    


}

TreeReader::~TreeReader() {

    inputFile->Close();
    delete inputFile;

    std::cout << "Destructor executed successfully." << std::endl;
}

bool TreeReader::Next() {

    amplitudesForEvent.clear();
    maxAmpl.clear();
    theSTE_time.clear();
    theCFD_time.clear();

    if ((theEventCount + 4) > (tree->GetEntries() )) {
        return false;
    } else {
        return true;
    }
}

void TreeReader::Histos() {

    Ch0 = new TH1D("Ch0", "Channel 0; Maximum Amplitude [V]; Counts", 500, 0.0, 0.185);  
    Ch1 = new TH1D("Ch1", "Channel 1; Maximum Amplitude [V]; Counts", 500, 0.0, 0.15);
    Ch2 = new TH1D("Ch2", "Channel 2; Maximum Amplitude [V]; Counts", 500, 0.0, 0.15);
    Ch3 = new TH1D("Ch3", "Channel 3; Maximum Amplitude [V]; Counts", 500, 0.0, 0.22);
    //if (!Ch3) std::cerr << "Failed to create histogram Ch3" << std::endl;

    STE0 = new TH1D("STE0", "Channel 0; Time of Arrival [s]; Counts", 50, 260*std::pow(10, -10), 310*std::pow(10, -10));
    STE1 = new TH1D("STE1", "Channel 1; Time of Arrival [s]; Counts", 50, 285*std::pow(10, -10), 335*std::pow(10, -10));
    STE2 = new TH1D("STE2", "Channel 2; Time of Arrival [s]; Counts", 50, 285*std::pow(10, -10), 335*std::pow(10, -10));
    STE3 = new TH1D("STE3", "Channel 3; Time of Arrival [s]; Counts", 50, 225*std::pow(10, -10), 275*std::pow(10, -10));

    CFD0 = new TH1D("CFD0", "Channel 0; Time of Arrival [s]; Counts", 50, 265*std::pow(10, -10), 315*std::pow(10, -10));
    CFD1 = new TH1D("CFD1", "Channel 1; Time of Arrival [s]; Counts", 50, 285*std::pow(10, -10), 335*std::pow(10, -10));
    CFD2 = new TH1D("CFD2", "Channel 2; Time of Arrival [s]; Counts", 50, 285*std::pow(10, -10), 335*std::pow(10, -10));
    CFD3 = new TH1D("CFD3", "Channel 3; Time of Arrival [s]; Counts", 50, 225*std::pow(10, -10), 275*std::pow(10, -10));
    
    STE0to1 = new TH1D("STE0to1", "STE Histogram 0to1; Time Difference [s]; Counts",300, -150*std::pow(10, -10), 150*std::pow(10, -10));
    STE0to2 = new TH1D("STE0to2", "STE Histogram 0to2; Time Difference [s]; Counts",300, -150*std::pow(10, -10), 150*std::pow(10, -10));
    STE0to3 = new TH1D("STE0to3", "STE Histogram 0to3; Time Difference [s]; Counts",300, -150*std::pow(10, -10), 150*std::pow(10, -10));
    STE1to2 = new TH1D("STE1to2", "STE Histogram 1to2; Time Difference [s]; Counts",300, -150*std::pow(10, -10), 150*std::pow(10, -10));
    STE1to3 = new TH1D("STE1to3", "STE Histogram 1to3; Time Difference [s]; Counts",300, -150*std::pow(10, -10), 150*std::pow(10, -10));
    STE2to3 = new TH1D("STE2to3", "STE Histogram 2to3; Time Difference [s]; Counts",300, -150*std::pow(10, -10), 150*std::pow(10, -10));

    CFD0to1 = new TH1D("CFD0to1", "CFD Histogram 0to1; Time Difference [s]; Counts",300, -150*std::pow(10, -10), 150*std::pow(10, -10));
    CFD0to2 = new TH1D("CFD0to2", "CFD Histogram 0to2; Time Difference [s]; Counts",300, -150*std::pow(10, -10), 150*std::pow(10, -10));
    CFD0to3 = new TH1D("CFD0to3", "CFD Histogram 0to3; Time Difference [s]; Counts",300, -150*std::pow(10, -10), 150*std::pow(10, -10));
    CFD1to2 = new TH1D("CFD1to2", "CFD Histogram 1to2; Time Difference [s]; Counts",300, -150*std::pow(10, -10), 150*std::pow(10, -10));
    CFD1to3 = new TH1D("CFD1to3", "CFD Histogram 1to3; Time Difference [s]; Counts",300, -150*std::pow(10, -10), 150*std::pow(10, -10));
    CFD2to3 = new TH1D("CFD2to3", "CFD Histogram 2to3; Time Difference [s]; Counts",300, -150*std::pow(10, -10), 150*std::pow(10, -10));
    //std::cout<< "OK1" << std::endl;
    channelHistograms = {Ch0, Ch1, Ch2, Ch3};
    //std::cout<< "OK2" << std::endl;
    steHistograms = {STE0, STE1, STE2, STE3};
    
    steHistogramsDiff = {STE0to1, STE0to2, STE0to3, STE1to2, STE1to3, STE2to3};

        // Wypełnianie listy histogramami CFD
    cfdHistogramsDiff = { CFD0to1, CFD0to2, CFD0to3, CFD1to2, CFD1to3, CFD2to3};

    cfdHistograms ={CFD0, CFD1, CFD2, CFD3};

}

void TreeReader::PrintTheFixedThreshold(){
    for (int i = 0; i < 4; ++i){
	std::cout << "Channel " << i << ": " << theFixedThreshold.at(i) << std::endl;
    }
}

void TreeReader::SaveHistos() {
    
    outputFile = new TFile("histos.root", "RECREATE");

    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Failed to create the output file" << std::endl;
    } else {
        outputFile->cd();
        
        for (int i = 0; i < 4; ++i) {
            channelHistograms[i]->Write();
            delete channelHistograms[i];

            steHistograms[i]->Write();
            delete steHistograms[i];

            cfdHistograms[i]->Write();
            delete cfdHistograms[i];
        }

        for (int i = 0; i < 6; ++i) {
            steHistogramsDiff[i]->Write();
            delete steHistogramsDiff[i];

            cfdHistogramsDiff[i]->Write();
            delete cfdHistogramsDiff[i];
        }

        outputFile->Close();
        delete outputFile;

        std::cout << "Histograms were saved successfully." << std::endl;
    }
}

void TreeReader::AnalyzeTrg(){
    for (int ch_iter = 0; ch_iter < 4; ++ch_iter) {
        
        //std::cout<< "Trigger: " << theEventCount / 4 << " , channel: " << ch_iter << std::endl;
        tree->GetEntry(theEventCount);

        if (channelNumber < 0 || channelNumber > 3) {
            std::cerr << "Channel number out of range. Must be 0, 1, 2, or 3." << std::endl;
            continue;
        }
        
	    amplitudesForEvent.push_back(*pAmplSamples);
        
        auto minElement = std::min_element(pAmplSamples -> begin(), pAmplSamples -> end()); 


        for ( int iter = 0; iter < pAmplSamples->size(); ++iter) {

            if ( pAmplSamples -> at(iter) < theConstantFrac * (*minElement) && theCFD_time.size() < (ch_iter+1) ) {
                
                theCFD_time.push_back( iter * samplingPeriod);
            } 
            
            if ( (pAmplSamples -> at(iter)) < theFixedThreshold.at(ch_iter)/10 && theSTE_time.size() <(ch_iter+1) ) {
                theSTE_time.push_back( iter * samplingPeriod);
            }  

            if(theSTE_time.size() == (ch_iter+1) && theCFD_time.size() ==(ch_iter+1)){ 

                break;
            }

        }

        
                    
        if (theSTE_time.size() < ch_iter+1){ 

            theSTE_time.push_back(0);
        }

        if (theCFD_time.size() < ch_iter+1){ 

            theCFD_time.push_back(0);
        }

        

        maxAmpl.push_back(std::abs(*minElement));

	    theEventCount++;
    }
/*
    TH1D* Ch[4]  = {Ch0, Ch1, Ch2, Ch3};
    TH1D* CFD[4] = {CFD0, CFD1, CFD2, CFD3};
    TH1D* STE[4] = {STE0, STE1, STE2, STE3};
*/  
    int r = 0;
    for (int index =0; index < 4; ++index ){
        //std::cout<< "OK4" << std::endl;
        channelHistograms[index]->Fill(maxAmpl.at(index));
        if (theCFD_time.at(index) != 0 ) cfdHistograms[index]->Fill(theCFD_time.at(index));
        //std::cout<< "OK5" << std::endl;
        if (theSTE_time.at(index) != 0 ) steHistograms[index]->Fill(theSTE_time.at(index));
        //std::cout<< "OK6" << std::endl;
        for( int k = index+1; k <4 ; ++k){
            //std::cout<< "OK7" << std::endl;
            //std::cout<< cfdHistogramsDiff.size() << std::endl;
            if (theCFD_time.at(index) != 0 && theCFD_time.at(k)!= 0) cfdHistogramsDiff[r] -> Fill( theCFD_time.at(index) - theCFD_time.at(k) );
            //std::cout<< "OK8" << std::endl;
            if (theSTE_time.at(index) != 0 && theSTE_time.at(k)!= 0) steHistogramsDiff[r]->Fill(theSTE_time.at(index) - theSTE_time.at(k));
            r++;
        }
        
    }
    
    std::cout << "Analysis complete for 4 channels (event " << (theEventCount)/4 << ")." << std::endl;
}
