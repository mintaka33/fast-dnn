// opencv_forward.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include "model_data.h"
#include "test_data.h"

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace dnn;

int opencv_forward()
{
    Net net = readNet("mnist.pb", "", "tensorflow");
    net.setPreferableBackend(0);
    net.setPreferableTarget(1);

    int sz[] = { 1, 1, 28, 28 };
    Mat blob(4, sz, CV_32F);
    net.setInput(blob);

    Mat prob = net.forward();

    return 0;
}

int main()
{
    return 0;
}
