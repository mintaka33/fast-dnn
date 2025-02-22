#include "pch.h"
#include "mnist_data.h"
#include <fstream>

inline int convertEndian(int n)
{
    char result[4] = {};
    result[0] = (n & 0xff000000) >> 24;
    result[1] = (n & 0x00ff0000) >> 16;
    result[2] = (n & 0x0000ff00) >> 8;
    result[3] = (n & 0x000000ff) >> 0;
    return *(int*)result;
}

Mnist::Mnist()
{
    std::ifstream imageFile, labelFile;
    imageFile.open(imageFileName_.c_str(), std::ios::binary);
    labelFile.open(labelFileName_.c_str(), std::ios::binary);
    if (imageFile.is_open() && labelFile.is_open())
    {
        Prefix imgPrefix = {};
        imageFile.read((char*)&imgPrefix, sizeof(Prefix));
        imgPrefix.magic = convertEndian(imgPrefix.magic);
        imgPrefix.num = convertEndian(imgPrefix.num);
        imgPrefix.width = convertEndian(imgPrefix.width);
        imgPrefix.height = convertEndian(imgPrefix.height);
        int labelPrefix[2] = {};
        labelFile.read((char*)labelPrefix, sizeof(labelPrefix));
        labelPrefix[0] = convertEndian(labelPrefix[0]);
        labelPrefix[1] = convertEndian(labelPrefix[1]);
        if (imgPrefix.magic == 2051 && imgPrefix.width == imgWidth_ && imgPrefix.height == imgHeight_ && imgPrefix.num > 0 && 
            labelPrefix[0] == 2049 && labelPrefix[1] == imgPrefix.num)
        {
            for (int i=0; i<imgPrefix.num; i++)
            {
                TestImg* img = new TestImg;
                imageFile.read(&img->data[0][0], imgWidth_ * imgHeight_);
                labelFile.read(&img->label, 1);
                testImages_.push_back(img);
            }
        }

        labelFile.close();
        imageFile.close();
    }
}

Mnist::~Mnist()
{
    for (auto img: testImages_)
    {
        delete img;
    }
}

int Mnist::getTestData(int index, char** data, char* label)
{
    if (index >= testImages_.size())
    {
        return -1;
    }
    
    *data = &testImages_[index]->data[0][0];
    *label = testImages_[index]->label;
    return 0;
}

void Mnist::mergeImage()
{
    if (testImages_.size() != 10000)
    {
        return;
    }
    int index = 0;
    char *buf, *dst;
    buf = new char[28 * 28 * 100 * 100];
    if (!buf)
    {
        return;
    }

    for (int y = 0; y < 100; y++)
    {
        for (int x = 0; x < 100; x++)
        {
            index = y * 100 + x;
            dst = buf + y * (28 * 28 * 100) + x * 28;
            for (int j=0; j<28; j++)
            {
                for (int i=0; i<28; i++)
                {
                    dst[j * 28 * 100 + i] = testImages_[index]->data[j][i];
                }
            }
        }
    }
    std::ofstream outfile;
    outfile.open("merge.yuv", std::ios::binary);
    if (outfile.is_open())
    {
        outfile.write(buf, 28 * 100 * 28 * 100);
        outfile.close();
    }

    delete[] buf;
}

