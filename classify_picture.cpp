#include "TProfile2D.h"
#include "TBuffer.h"
#include "TMath.h"
#include "THLimitsFinder.h"
#include "TError.h"
#include "TClass.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <TCanvas.h>
#include <TH1D.h>
 
Bool_t TProfile2D::fgApproximate = kFALSE;

//adc average 2D
ClassImp(TProfile2D);
void classify_picture(){
  auto fid_1_1_frame_4 = new TProfile2D("c1", "fid_1_1_frame_4", 356, 0, 356, 512, 0, 512, "");



  string file_path="./filter_date_test_2_0123_correct.txt";
  

  ifstream fp0(file_path);
  ifstream &filePtr = fp0;
  if(!fp0)
        cout <<  "open file " + file_path + " error!"  << endl;
  
  while (!filePtr.eof()){
     Double_t frame_indexer;  Double_t area_scan_sift;Double_t row_coordinate;
     Double_t col_coordinate; Double_t adc_date;


    filePtr >> frame_indexer >> area_scan_sift >> row_coordinate >> col_coordinate >> adc_date;
    fid_1_1_frame_4->Fill(row_coordinate, col_coordinate, adc_date); 	
  }
  fp0.close();
     
   gStyle->SetOptStat(1);
   gStyle->SetCanvasColor(33);
   gStyle->SetFrameFillColor(18);
   Float_t xMin=0, yMin=0, xMax=356, yMax=512;
   Int_t cancolor = 17;
   auto c1h = new TCanvas("c1h","fid_1_1_frame_4",10,10,800,600);
   fid_1_1_frame_4->Draw("COLZ");
   c1h->Update();
   //c1h->Print("./fid_1_1_frame_4.pdf");
   delete c1h;







  //adc standard deviation 2D
  const int row_max = 356;    // 行数
  const int col_max = 512;    // 列数
  double adc_mean;            //能量均值
  double noise_stddev;        //噪声

  auto fid_1_1_frame_4_noise = new TProfile2D("c2", "fid_1_1_frame_4_noise", 356, 0, 356, 512, 0, 512, "");
  
  // 创建一个三维向量，存储 ADC 值(未做)
  // 创建一个容器来储存噪声的值(未做)
  // 提取标准差并打印
    for (int row = 1; row <= row_max; ++row) {
        for (int col = 1; col <= col_max; ++col) {
            noise_stddev = fid_1_1_frame_4->GetBinError(row, col); // TProfile2D 中的误差即为标准差
            fid_1_1_frame_4_noise->Fill(row, col, noise_stddev);
        }
    }

  //画出二维分布图形
   auto c2h = new TCanvas("c2h","fid_1_1_frame_4_noise",10,10,800,600);
   fid_1_1_frame_4_noise->Draw("COLZ");
   c2h->Update();
   //c2h->Print("./fid_1_1_frame_4_noise.pdf");
   delete fid_1_1_frame_4_noise;
   delete c2h;
  
  //画出噪声的正态分布
    TCanvas* c3 = new TCanvas("c3", "Noise_Distribution", 800, 600);
    TH1D* histNoise = new TH1D("histNoise", "Noise Distribution", 100, 0, 30); // 假设噪声范围是0到10
    
    for (int row = 1; row <= row_max; ++row) {
        for (int col = 1; col <= col_max; ++col) {
            noise_stddev = fid_1_1_frame_4->GetBinError(row, col); // TProfile2D 中的误差即为标准差
            histNoise->Fill(noise_stddev);
        }
    }
    delete fid_1_1_frame_4;

    // 在画布上绘制直方图和拟合曲线
    c3->SetGrid();//为当前画布设置网格
    gStyle->SetOptStat(1);
    histNoise->Draw();
        // 添加标题和轴标签
    histNoise->GetXaxis()->SetTitle("Noise Size");
    histNoise->GetYaxis()->SetTitle("Pixel Count");
    

    // 显示画布
    c3->Update();
    //c3->WaitPrimitive();

    // 保存图形到文件
    c3->Print("./Noise_Distribution.pdf");

    // 清理
    delete histNoise;
    delete c3;
}
  