//标准库 输入流 字符串 时间 容器
#include <iostream>  
#include <fstream> 
#include <string>  
#include <vector>
#include <unistd.h>  // 包含 getopt 函数的头文件
#include <cstdlib>   // 包含 exit 函数的头文件

using namespace std;


int main(){
  string file_path="./data_1.bin";


  // input from a file
  fstream fp0(file_path, ios::in|ios::binary);//标准模式打开二进制读入，一组数据就是32位二进制，4byte
  if(!fp0)
        cout <<  "open file " + file_path + " error!"  << endl;

    fp0.seekg(0,ios::end);
    size_t file_size = fp0.tellg();
    fp0.seekg(0,ios::beg);
    int ope_bytes = 4;
    int n_total_data_number=file_size/ope_bytes;//总的数据的数量n_total_data_number就是文件大小除以4byte
    cout<<1<<endl;

    int16_t frame_indexer;  int16_t area_scan_sift;int16_t row_coordinate;
    int16_t col_coordinate; int16_t adc_date;int32_t adc_temp;
    cout<<2<<endl;



    char* buf = new char[ope_bytes];//在堆中定义buf是ope_bytes的地址，buf本身的大小是1byte，而指针指向的是大小为ope_bytes 的值的字符空间，决定了在堆上分配的内存块的大小，而不是 buf 指针的数量。
    cout<<3<<endl;
    int num = 0;

    ofstream fp1("./filter_date_test.txt");
        // 检查文件是否成功打开
    if (!fp1) {
        std::cerr << "Unable to open file!" << std::endl;
        return 1;
    }
    
    
    for(int i=0;i < file_size/ope_bytes;i++)
        {   
        fp0.read((char *)buf, sizeof(buf[0])*ope_bytes);//读取四个字节的数到buf
          
            adc_temp=(buf[2] & 0X000000FF) | ((buf[3] & 0X000000FF) << 8 ) | ((buf[0] & 0X000000FF) << 16 ) | ((buf[1] & 0X000000FF) << 24 );
            
            if(adc_temp !=0)  
            {
                frame_indexer= (adc_temp  &  0x80000000) >> 31;//只取最高位然后右移31位
                area_scan_sift=(adc_temp  &  0x40000000) >> 30;
                row_coordinate=(adc_temp  &  0x3FF00000) >> 21;
                col_coordinate=(adc_temp  &  0x001FF000) >> 12;
                adc_date=(adc_temp  & 0x00000FFF);

                if(adc_date & 0x0800) //0000 1000 0000 0000
                    adc_date |= 0xF000;
                
                fp1 << frame_indexer << "  " << area_scan_sift << "  " << row_coordinate<< "  " << col_coordinate << "  " << adc_date << "  " <<endl;
                fp1 << endl; // 每行结束后换行
            }
            
            
        } 
    fp1.close();    
    delete[]buf;
    cout<<4<<endl;
    fp0.close();

    


    return 0;

}