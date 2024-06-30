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
#include "TCanvas.h"
#include <TH1D.h>
 
Bool_t TProfile2D::fgApproximate = kFALSE;    
    // 自定义高斯函数
    double gaussian(double *x, double *par) {
           // par[0] 是振幅（Amplitude）
           // par[1] 是均值（Mean）
           // par[2] 是标准差（Standard Deviation）
           double amplitude = par[0];
           double mean = par[1];
           double sigma = par[2];
    
           // 高斯函数的公式
           double exponent = (x[0] - mean) / sigma;
           double gauss = amplitude * std::exp(-0.5 * exponent * exponent);
    
        return gauss;
    }
    

//adc average 2D
ClassImp(TProfile2D);
void classsify_Noise_Distribution(){
  auto fid_1_1_frame_4 = new TProfile2D("", "fid_1_1_frame_4", 356, 0, 356, 512, 0, 512, "");



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

  //adc standard deviation 2D
  const int row_max = 356;    // 行数
  const int col_max = 512;    // 列数
  double adc_mean;            //能量均值
  double noise_stddev;        //噪声
  //画出噪声的正态分布
    TCanvas* c3 = new TCanvas("c3", "Noise_Distribution", 800, 600);
    TH1D* histNoise = new TH1D("Noise_Distribution", "Noise_Distribution", 100, 0, 30); // 假设噪声范围是0到10
    
    for (int row = 1; row <= row_max; ++row) {
        for (int col = 1; col <= col_max; ++col) {
            noise_stddev = fid_1_1_frame_4->GetBinError(row, col); // TProfile2D 中的误差即为标准差
            histNoise->Fill(noise_stddev);
        }
    }


    // 在画布上绘制直方图和拟合曲线
    c3->SetGrid();//为当前画布设置网格
    histNoise->Draw();
        // 添加标题和轴标签
    histNoise->GetXaxis()->SetTitle("Noise Size");
    histNoise->GetYaxis()->SetTitle("Pixel Count");   

    //通过高斯函数拟合

    // 设置参数名称和初步估计值
    //gauss_func.SetParameters(1.0, 0.0, 1.0); // 初步估计参数：[0] = amplitude, [1] = mean, [2] = sigma
    // 高斯函数定义
    TF1* noise_gauss_func = new TF1 ("noise_gauss_func", gaussian,0,30,3);

    // 设置高斯函数的参数名称和初步估计值
    noise_gauss_func->SetParameters(1.0, 3.0, 1.0);  // [0] = amplitude, [1] = mean, [2] = sigma
    noise_gauss_func->SetParNames("Constant", "Mean", "Sigma");
    noise_gauss_func->SetLineColor(2);
    //拟合
    histNoise->Fit(noise_gauss_func);
    double amplitude = noise_gauss_func->GetParameter(0);
    double mean = noise_gauss_func->GetParameter(1);
    double sigma = noise_gauss_func->GetParameter(2);
    noise_gauss_func->Draw("same");
    std::cout << "Amplitude: " << amplitude << ", Mean: " << mean << ", Sigma: " << sigma << std::endl;

    // 创建一个TPaveText对象，用于在统计框下面显示拟合参数
    /*TPaveText *pt = new TPaveText(0.7, 0.6, 0.9, 0.7, "NDC");
    pt->SetFillColor(0);
    pt->SetTextAlign(12);
    pt->AddText(Form("Amplitude: %.3f", amplitude));
    pt->AddText(Form("Mean: %.3f", mean));
    pt->AddText(Form("Sigma: %.3f", sigma));
    pt->Draw("same");*/
    gStyle->SetOptFit(0002);  // 显示拟合参数

    // 显示画布
    c3->Update();
    //c3->WaitPrimitive();

    // 保存图形到文件
    c3->Print("./Noise_Distribution.pdf");
    delete fid_1_1_frame_4;
    delete histNoise;
    delete c3;

}