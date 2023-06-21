//
//  treegen.C
//  
//
//  Created by Jim Drachenberg on 6/13/22.
//  Copyright 2022. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>

//void treegen()
int treegen()
{

	char hist    [100];
	char name    [200];
	char filename[100];
	char title   [100];
	char label   [100];

    float nat_slope = 1.5; // "natural" (data) spectrum power law
    float syn_slope = 2.5; // "synthetic" (simu) spectrum power law

    //===== "natural" (data) reco params ====//
    float nat_pt_gen = -999.;
    float nat_pt_smear = -999.;
    float nat_shift = 0.;
    float a_nat_shift = 0.3;
    float s_nat_shift = 0.02;
    float s_nat_smear = 0.3;

    //===== "synthetic" (simu) reco params ====//
    float syn_pt_gen = -999.;
    float syn_pt_smear = -999.;
    float syn_shift = 0.;
    float a_syn_shift = 0.3;
    float s_syn_shift = 0.02;
    float s_syn_smear = 0.3;

    TTree *tree_nat = new TTree("tree_nat","Natural Jet Tree");
    TBranch *br_nat_pt_gen = tree_nat->Branch("nat_pt_gen",&nat_pt_gen);
    TBranch *br_nat_pt_smear = tree_nat->Branch("nat_pt_smear",&nat_pt_smear);

    TTree *tree_syn = new TTree("tree_syn","Synthetic Jet Tree");
    TBranch *br_syn_pt_gen = tree_syn->Branch("syn_pt_gen",&syn_pt_gen);
    TBranch *br_syn_pt_smear = tree_syn->Branch("syn_pt_smear",&syn_pt_smear);

    TF1 *fJetXsec_nat = new TF1( "fJetXsec_nat","[0]/(x^[1])",1,100 );
    fJetXsec_nat->SetParameters(1e4,nat_slope);

    TF1 *fJetXsec_syn = new TF1( "fJetXsec_syn","[0]/(x^[1])",1,100 );
    fJetXsec_syn->SetParameters(1e4,syn_slope);

    TRandom1 *rshift_nat = new TRandom1();
    TRandom1 *rsmear_nat = new TRandom1();
    TRandom1 *rshift_syn = new TRandom1();
    TRandom1 *rsmear_syn = new TRandom1();
//    rshift->SetSeed(1);
//    rsmear->SetSeed(2);

    for (int i = 0;i<1e5;i++) {
        nat_pt_gen = fJetXsec_nat->GetRandom();
        syn_pt_gen = fJetXsec_syn->GetRandom();

        //===== now, let's shift and smear the jet p_T ======//
        nat_shift = a_nat_shift*rshift_nat->Gaus( nat_pt_gen, s_nat_shift*nat_pt_gen );
        nat_pt_smear = rsmear_nat->Gaus( nat_pt_gen+nat_shift, s_nat_smear*nat_pt_gen );
        tree_nat->Fill();

        syn_shift = a_syn_shift*rshift_syn->Gaus( syn_pt_gen, s_syn_shift*syn_pt_gen );
        syn_pt_smear = rsmear_syn->Gaus( syn_pt_gen+syn_shift, s_syn_smear*syn_pt_gen );
        tree_syn->Fill();

//        hJetXsec_reco->Fill(pt_reco);
    }

    TFile* outputFile = TFile::Open( "jet_spectrum_omni_test.root", "RECREATE" );
    tree_nat->Write();
    tree_syn->Write();
    outputFile->Write();
    outputFile->Close();

    return 0;

}