#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <arm_neon.h>
#include <chrono>

using namespace std;
using namespace cv;

void AddImageNEON(uint8_t *dst, uint8_t const *src1, uint8_t const *src2, int width, int height)
{
    int const vectorNumberPerRow = width / 16;

    for (int y = 0; y < 3 * height; y++)
    {
        for (int i = 0; i < vectorNumberPerRow; i++)
        {

            uint8x16_t a = vld1q_u8(src1);
            uint8x16_t b = vld1q_u8(src2);

            uint8x16_t r = vqaddq_u8(a, b);

            vst1q_u8(dst, r);

            src1 += 16;
            src2 += 16;
            dst += 16;
        }
    }
}

void SubImageNEON(uint8_t *dst, uint8_t const *src1, uint8_t const *src2, int width, int height)
{
    int const vectorNumberPerRow = width / 16;

    for (int y = 0; y < 3 * height; y++)
    {
        for (int i = 0; i < vectorNumberPerRow; i++)
        {

            uint8x16_t a = vld1q_u8(src1);
            uint8x16_t b = vld1q_u8(src2);

            uint8x16_t r = vqsubq_u8(a, b);

            vst1q_u8(dst, r);

            src1 += 16;
            src2 += 16;
            dst += 16;
        }
    }
}

int main()
{
    Mat image1 = imread("images/img320_1.jpg");
    Mat image2 = imread("images/img320_1.jpg");

    if (image1.empty() || image2.empty())
    {
        std::cerr << "Failed to load images." << std::endl;
        return 1;
    }

    if (image1.size() != image2.size())
    {
        std::cerr << "Images must have the same dimensions." << std::endl;
        return 1;
    }

    auto t1 = chrono::high_resolution_clock::now();
    Mat result(image1.rows, image1.cols, CV_8UC3, Scalar(2));
    for (int i = 1; i < 100; i++)
    {
        AddImageNEON(result.data, image1.data, image2.data, image1.cols, image1.rows);
    }

    auto t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    float avg = duration / 100;
    cout << "estimated_time" << endl;
    cout << duration << " us" << endl;
    cout << "avg " << avg << endl;

    Mat result2(image1.rows, image1.cols, CV_8UC3, Scalar(2));
    SubImageNEON(result2.data, image1.data, image2.data, image1.cols, image1.rows);

    imshow("Image 1", image1);
    imshow("Image 2", image2);
    imshow("Summed Image NEON", result);
    imshow("Raznitsa image", result2);

    waitKey(0);

    return 0;
}