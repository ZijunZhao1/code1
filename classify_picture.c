void classify_picture(){
  TCanvas* c1 = new TCanvas("c1", "c1", 700, 500);
  TH1D* fid_1_1fram4 = new TH1D("fid_1_1fram4", "", 356, 0, 356,512,0,512);
  
  fid_1_1fram4->Fill(row, col )
  fid_1_1fram4->Draw();
}
