#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "OpenCL.h"
#include <windows.h>

using namespace cv;

int main(int argc, char **argv)
{
	ShowAvailableDevicesCL();
	InitializeCL();

	VideoCapture cap(0);
	Mat frame,res;

	cap >> frame;
	res = Mat(frame.rows, frame.cols, CV_8U, Scalar(0));
	GetMemoryCL(sizeof(unsigned char) * frame.rows * frame.cols * 3,
		sizeof(unsigned char) * res.rows * res.cols * 1, frame.rows, frame.cols);

	LARGE_INTEGER clock, start, end;
	QueryPerformanceFrequency(&clock);

	while (waitKey(1000/60) < 1)
	{
		cap >> frame;
		WriteSourceCL(frame.data);

		QueryPerformanceCounter(&start);

		for (int i = 0; i < 1000; i++)
			ProcessCL();
			//cvtColor(frame, res, COLOR_BGR2GRAY);

		QueryPerformanceCounter(&end);

		ReadResultCL(res.data);
		printf("%lf[ms]\n", (double)(end.QuadPart - start.QuadPart) * 1000.0 / clock.QuadPart);
		imshow("", res);
	}

	ReleaseMemoryCL();
	cap.release();
}