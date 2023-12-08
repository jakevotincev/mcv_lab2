#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>

using namespace cv;
using namespace std;

void AddImage(uint8_t *dst, uint8_t const *src1, uint8_t const *src2, int width, int height)
{
    int index = 0;
    for (int y = 0; y < height * 3; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int t = src1[index] + src2[index];

            dst[index] = t > 255 ? 255 : t;

            index++;
        }
    }
}

void SubImage(uint8_t *dst, uint8_t const *src1, uint8_t const *src2, int width, int height)
{
    int index = 0;
    for (int y = 0; y < height * 3; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int t = src1[index] - src2[index];

            dst[index] = t < 0 ? 0 : t;

            index++;
        }
    }
}

int main()
{
    Mat image1 = imread("images/im1920_1.jpg");
    Mat image2 = imread("images/im1920_2.jpg");

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

    Mat result(image1.rows, image1.cols, CV_8UC3, cv::Scalar(2));
    for (int i = 1; i < 100; i++)
    {
        AddImage(result.data, image1.data, image2.data, image1.cols, image1.rows);
    }

    auto t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(t2- t1).count();
    float avg = duration / 100;
    cout << "estimated_time" << endl;
    cout << duration << " us" << endl;
    cout << "avg " << avg << endl;

    Mat result2(image1.rows, image1.cols, CV_8UC3, cv::Scalar(2));
    SubImage(result2.data, image1.data, image2.data, image1.cols, image1.rows);

    imshow("Image 1", image1);
    imshow("Image 2", image2);
    imshow("Summed Image", result);
    imshow("Raznitsa image", result2);

    waitKey(0);

    return 0;
}