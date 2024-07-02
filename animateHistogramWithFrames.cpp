#include <TCanvas.h>
#include <TH2D.h>
#include <TH1.h>
#include <TSystem.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <TApplication.h>
#include <TProfile.h>

void animateHistogramWithFrames() {
    // 创建一个画布
    TCanvas *canvas = new TCanvas("canvas", "animateHistogramWithFrames_frame", 800, 600);

    string file_path="./filter_date_test.txt";
  

    ifstream fp0(file_path);
    ifstream &filePtr = fp0;
    if(!fp0)
        cout <<  "open file " + file_path + " error!"  << endl;

    // 变量定义

    Double_t frame_indexer;  Double_t area_scan_sift;Double_t row_coordinate;
    Double_t col_coordinate; Double_t adc_date;
    std::vector<TProfile2D*> frames;
    
    int histCounter = 0;
    std::ostringstream histName;
    histName << "hist_" << histCounter;
    //对于容器中的对象最好用指针的方式代替
    auto *currentHist = new TProfile2D(histName.str().c_str(), "animateHistogramWithFrames_frame", 356, 0, 356, 512, 0, 512, "");


    int a[5];int b[5]; int c;//a是所要读的那一行数据，当a=c（a=c ！=b的时候结束一帧的填图）
    filePtr >> frame_indexer >> area_scan_sift >> row_coordinate >> col_coordinate >> adc_date;
    a[0]=frame_indexer;a[1]=area_scan_sift;a[2]=row_coordinate;a[3]=col_coordinate;a[4]=adc_date;
    b[0]=a[0]; b[1]=a[1];b[2]=a[2];b[3]=a[3];b[4]=a[4];
    c=a[0];

    int d=0;//处理数据的最开头一行，之前给d赋值0,就是第一次循环的数据不读入，因为开始的时候a[]=b[],然后会让第一行数据两次填入图一中；

    while (!filePtr.eof()){

            

            //创建第一个图
            currentHist = new TProfile2D(histName.str().c_str(), "animateHistogramWithFrames_frame", 356, 0, 356, 512, 0, 512, "");                   
            
            while (a[0] == c ){

                currentHist->Fill(a[2], a[3], a[4]);
                c = a[0];
                a[0]=b[0]; a[1]=b[1];a[2]=b[2];a[3]=b[3];a[4]=b[4];
                //判断文件没有读到末尾，仍然有下一行，如果没有下一行就break出while(a[0] == c )循环
                if(filePtr.eof()){
                    b[0]=a[0];break;
                }
                filePtr >> frame_indexer >> area_scan_sift >> row_coordinate >> col_coordinate >> adc_date;
                b[0]=frame_indexer;b[1]=area_scan_sift;b[2]=row_coordinate;b[3]=col_coordinate;b[4]=adc_date;
                //处理数据的最开头一行，之前给d赋值0,就是第一次循环的数据不读入，因为开始的时候a[]=b[],然后会让第一行数据两次填入图一中；
                if(d==0){
                    d=1;
                    currentHist->Reset();//数据清除
                }

            }
                frames.push_back(currentHist);//将生成的图填入容器中的图
                histName.str("");//先前的图的名称清除
                histName << "hist_" << ++histCounter;
                c =a[0];
            
        }
        // 添加数据点到当前图表
    
    filePtr.close();

        // 动画循环，逐帧显示图表并保存每一帧的图像
    int frameNumber = 0;
    for (auto frame : frames) {
        // 清除画布并绘制当前帧
        canvas->Clear();
        frame->Draw("COLZ");

        // 更新画布
        canvas->Update();

        // 保存当前帧为图像文件
        std::ostringstream filename;
        filename << "frame_" << std::setw(4) << std::setfill('0') << frameNumber++ << ".png";
        canvas->SaveAs(filename.str().c_str());

        // 延时以创建动画效果
        gSystem->Sleep(2000); // 延时 500 毫秒
    }

    // 释放内存
    for (auto frame : frames) {
        delete frame;
    }
}