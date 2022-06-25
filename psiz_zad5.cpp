// psiz_zad5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include <fstream>
#include <limits>
#include <filesystem>
#include <regex>
namespace fs = std::filesystem;

struct BMP_header
{
    char name[2];
    unsigned int size;
    unsigned short int reserved_1;
    unsigned short int reserved_2;
    unsigned int offset;
}header_bmp;

struct DIB_header {
    unsigned int headerSize;
    unsigned int width;
    unsigned int height;
    unsigned short int planes;
    unsigned short int bitPerPixel;
    unsigned int compresion;
    unsigned int imageSize;
    unsigned int xPelsPerMeter;
    unsigned int yPelsPerMeter;
    unsigned int colorUsed;
    unsigned int colorImportant;

}header_dib;

void info(const char* file) {

    FILE* fp = fopen(file, "rb");

    if (fp == nullptr) {
        std::cout << "ERR cant open file: " << file << "\n";
    }
    else {


        std::cout << "############################ \n";
        std::cout << "###### File info  \n" << "\n\n";

        // Header BMP

        fread(&header_bmp.name, 2, 1, fp);
        fread(&header_bmp.size, sizeof(header_bmp.size), 1, fp);
        fread(&header_bmp.reserved_1, sizeof(header_bmp.reserved_1), 1, fp);
        fread(&header_bmp.reserved_2, sizeof(header_bmp.reserved_2), 1, fp);
        fread(&header_bmp.offset, sizeof(header_bmp.offset), 1, fp);


        std::cout << "File name:" << file << "\n";
        std::cout << "fileSize: " << header_bmp.size << " bytes\n";
        std::cout << "2 first symbols :" << (char)header_bmp.name[0] << header_bmp.name[1] << "\n";
        std::cout << "Reserved1: " << header_bmp.reserved_1 << "\n";
        std::cout << "Reserved2: " << header_bmp.reserved_1 << "\n";
        std::cout << "Offset : " << header_bmp.offset << "\n";

        fread(&header_dib, sizeof(struct DIB_header), 1, fp);

        std::cout << "Image Width: " << header_dib.width << " px\n";
        std::cout << "Image Height: " << header_dib.height << " px\n";
        std::cout << "Bin header info: " << header_dib.headerSize << "\n";
        std::cout << "Bits per pixel: " << header_dib.bitPerPixel << "\n";
        std::cout << "Planes: " << header_dib.planes << "\n";
        std::cout << "Compression: " << header_dib.compresion << "\n";
        std::cout << "Image size: " << header_dib.imageSize << "\n";
        std::cout << "Vertical res: " << header_dib.xPelsPerMeter << "\n";
        std::cout << "Horizontal res: " << header_dib.yPelsPerMeter << "\n";
        std::cout << "Num of colors " << header_dib.colorUsed << "\n";
        std::cout << "Num of important colors: " << header_dib.colorImportant << "\n";

        fclose(fp);
    }
}

void convertToNegativeImage(const char* file) {

    FILE* origFile = fopen(file, "rb");

    std::string newFileName("negative_");
    newFileName.append(file);

    FILE* newFile = fopen(newFileName.data(), "wb");
    if (newFile == nullptr) {
        std::cout << "\n Cant open the file!";
    }
    else {

        fwrite(&header_bmp.name[0], 1, 1, newFile);
        fwrite(&header_bmp.name[1], 1, 1, newFile);
        fwrite(&header_bmp.size, sizeof(header_bmp.size), 1, newFile);
        fwrite(&header_bmp.reserved_1, sizeof(header_bmp.reserved_1), 1, newFile);
        fwrite(&header_bmp.reserved_2, sizeof(header_bmp.reserved_2), 1, newFile);
        fwrite(&header_bmp.offset, sizeof(header_bmp.offset), 1, newFile);

        fwrite(&header_dib, sizeof(struct DIB_header), 1, newFile);

        //credits: https://github.com/drinzmz/bmp/blob/main/bmp/main.cpp
        std::cout << "\n Converting... Wait...";
        int bmpImg;
        for (int i = header_bmp.offset; i < header_bmp.size; i++)
        {
            fseek(origFile, i, SEEK_SET);
            fseek(newFile, i, SEEK_SET);
            fread(&bmpImg, 3, 1, origFile);
            bmpImg = INT_MAX - bmpImg;
            fwrite(&bmpImg, 3, 1, newFile);
        }

        std::fclose(origFile);
        std::fclose(newFile);
        std::cout << "\n Conversion successfull! Check out: " << newFileName << "\n";

    }
}

void main(int argc, char* argv[])
{
    const char* file;

    if (argv[1] != nullptr) {
        file = argv[1];
        info(file);
        convertToNegativeImage(file);
    }
    else {
        //batch processing for all files found
        std::string path("./");
        std::string ext(".bmp");
        std::regex pat{ "negative" };
        for (auto& p : fs::recursive_directory_iterator(path))
        {
            if (!regex_search(p.path().stem().string(),pat) && p.path().extension() == ext) {

                info((p.path().stem().string() + ext).data());
                convertToNegativeImage((p.path().stem().string() + ext).data());
            }
                
        }
    }
    
}





// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
