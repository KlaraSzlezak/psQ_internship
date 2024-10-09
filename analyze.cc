#include "TreeReader.h"

int main() {
   
    TreeReader reader;

    reader.Histos();
    while( reader.Next() ){
        reader.AnalyzeTrg();
        //break;
    }
    reader.PrintTheFixedThreshold();
    reader.SaveHistos();

    return 0;
}

//g++ -o TreeReader analyze.cc TreeReader.cc `root-config --cflags --libs`
