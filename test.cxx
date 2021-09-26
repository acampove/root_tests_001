#include <chrono>

#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TSystem.h>
#include <TChain.h>
#include <TRandom3.h>
#include "ROOT/RDataFrame.hxx"

//----------------------------------------------
void make_input(const unsigned &nentries, const TString &filepath)
{
    TRandom3 ran(1);
    TFile file(filepath, "recreate");
    TTree tree("tree", "");

    float x, y, z;

    tree.Branch("x", &x, "x/F");
    tree.Branch("y", &y, "y/F");
    tree.Branch("z", &z, "z/F");

    for (unsigned i_entry = 0; i_entry < nentries; i_entry++)
    {
	x = ran.Uniform(1);
	y = ran.Exp(1);
	z = ran.Gaus(0, 1);

	tree.Fill();
    }

    tree.Write();
    file.Close();
}
//----------------------------------------------
TTree *get_tree_add(const TString &filepath, const TString &id)
{
    TString ofilepath = Form("/tmp/temp_%s.root", id.Data());

    ROOT::RDataFrame df("tree", filepath);
    auto df_1 = df.Define("id", id.Data());
    df_1.Snapshot("tree", ofilepath);

    TFile *ifile = new TFile(ofilepath);
    TTree *itree = (TTree*)ifile->Get("tree");

    return itree;
}
//----------------------------------------------
void merge_save(TTree *tree_1, TTree *tree_2)
{
    TList l_tree;
    l_tree.Add(tree_1);
    l_tree.Add(tree_2);

    TFile ofile("file_3.root", "recreate");
    auto tree_3 = TTree::MergeTrees(&l_tree);
    tree_3->Write();
    ofile.Close();
}
//----------------------------------------------
void merge_add_save(TChain *chain)
{
    TFile ofile("file_3.root", "recreate");

    auto tree_3 = chain->CloneTree(0);
    int id;
    tree_3->Branch("id", &id, "id/I");

    auto nentries = chain->GetEntries();
    for (unsigned i_entry = 0; i_entry < nentries; i_entry++)
    {
	chain->GetEntry(i_entry);
	id = chain->GetTreeNumber() + 1;
	tree_3->Fill();
    }

    tree_3->Write();
    ofile.Close();
}
//----------------------------------------------
int main(int argv, char *argc[])
{
    if (argv < 3)
	return 1;

    unsigned nentries = atoi(argc[1]);
    unsigned method   = atoi(argc[2]);
    //---------------
    //Make input
    //---------------
    make_input(nentries, "file_1.root");
    make_input(nentries, "file_2.root");

    auto start = clock();
    //---------------
    //Method 1
    //---------------
    if (method == 1)
    {
	auto tree_1 = get_tree_add("file_1.root", "1");
	auto tree_2 = get_tree_add("file_2.root", "2");

	merge_save(tree_1, tree_2);
    }
    //---------------
    //Method 2
    //---------------
    else if (method == 2)
    {
	TChain *chain = new TChain("tree");
	chain->Add("file_*.root");

	merge_add_save(chain);
    }
    //---------------
    //---------------
    auto end = clock();
    printf ("Time: %0.8f sec \n", ((float) end - start)/CLOCKS_PER_SEC);

    return 0;
}
//----------------------------------------------

