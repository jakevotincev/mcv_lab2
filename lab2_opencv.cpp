#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>
using namespace cv;
using namespace std;

int main()
{
    Mat image1 = imread("im1920_1.jpg");
    Mat image2 = imread("im1920_2.jpg");

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
    Mat result;
    add(image1, image2, result);
    auto t2 = chrono::high_resolution_clock::now();
    auto duration_neon = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    cout << "estimated_time" << endl;
    cout << duration_neon << " us" << endl;

    Mat result2;
    subtract(image1, image2, result2);

    imshow("Image 1", image1);
    imshow("Image 2", image2);
    imshow("Summed Image", result);
    imshow("Raznitsa image", result2);

    waitKey(0);

    return 0;
}