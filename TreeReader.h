#ifndef TREEREADER_H
#define TREEREADER_H

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"

class TreeReader {
public:
    // Contructor
    TreeReader(const std::string &inputFilePath = "/eos/project-h/h8ctb/2024_H8_TestBeams/H8TB_202405_PPSTiming_Diamond_QFib_psQ_Parasitic/PSQ/Runs/Reco_Runs/Run3.root",
               const std::string &treeName = "LeCroyTree");

    // a method to load the next 4 entries - the next event
    bool Next();

    void Histos();

    void SaveHistos();
    
    void PrintTheFixedThreshold();

    // to analyze a whole event, 4 channels
    void AnalyzeTrg();

    ~TreeReader();

private:

    TFile* inputFile;
    TFile* outputFile;
    TTree* tree;

    unsigned int theEventCount;
    
    //Maximum amplitude distribution
    TH1D *Ch0;
    TH1D *Ch1;
    TH1D *Ch2;
    TH1D *Ch3;

    std::vector<double> theFixedThreshold; //Time of arrival distribution - Single Threshold Estimation
    TH1D *STE0, *STE1, *STE2, *STE3;
    TH1D *STE0to1,*STE0to2, *STE0to3, *STE1to2, *STE1to3, *STE2to3;

    double theConstantFrac; //Time of arrival distribution - Constant Fraction Discrimination
    TH1D *CFD0, *CFD1, *CFD2, *CFD3;
    TH1D *CFD0to1,*CFD0to2, *CFD0to3, *CFD1to2, *CFD1to3, *CFD2to3;

    std::vector<TH1D*> steHistograms;
    std::vector<TH1D*> cfdHistograms;
    std::vector<TH1D*> steHistogramsDiff;
    std::vector<TH1D*> cfdHistogramsDiff;
    std::vector<TH1D*> channelHistograms;

    int channelNumber;
    std::vector<double> amplSamples;
    double samplingPeriod;
    double trgTime;
    double hOffset;

    std::vector<std::vector<double>> amplitudesForEvent; // 4x502
    std::vector<double> maxAmpl;
    
    std::vector<double> theSTE_time;
    std::vector<double> theCFD_time; 

    std::vector<double>* pAmplSamples;
};

#endif // TREEREADER_H


/*
Channel 0: -0.181267
Channel 1: -0.145013
Channel 2: -0.145013
Channel 3: -0.21752
*/
