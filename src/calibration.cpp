#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/objdetect/aruco_dictionary.hpp>
//*****************************
#include<string.h>
//*****************************
#include <opencv2/core/cvstd.hpp>
#include <opencv2/calib3d.hpp>
//*****************************
#include<opencv2/imgproc/types_c.h>
//*****************************

using namespace std;
using namespace cv;

int ImgCalibration()
{
	Mat image, img_gray;
	int boardsize[2]{ 6,6 };//棋盤格每行列角點個數//不太懂

	vector<vector<Point3f>>chess_point;//棋盤格角點的三維座標
	vector<Point3f>world_point;//三維世界座標
	vector<vector<Point2f>>image_point;//保存所有角點
	vector<Point2f>img_corrner_point;//保存偵測到的角點
	vector<String>images_path;//建立物件存放讀取路徑

	string image_path = "aoi/*.bmp";//待處理圖路徑
	glob(image_path, images_path);//讀取指定文件夾圖像

	//轉世界座標
	for (int i = 0; i < boardsize[1]; i++)
	{
		for (int j = 0; j < boardsize[0]; j++)
		{
			world_point.push_back(Point3f(j, i, 0));
		}

	}

	for (int i = 0; i < images_path.size(); i++)
	{
		image = imread(images_path[i]);
		
		cvtColor(image, img_gray, COLOR_BGR2GRAY);
		cout << "Image Path: " << images_path[i] << endl; 
		threshold(img_gray, img_gray, 100, 255, CV_THRESH_BINARY_INV);
		//imshow("D", img_gray);
		//waitKey(0);

		//檢測角點
		bool found_success = findChessboardCorners(img_gray, Size(boardsize[0], boardsize[1]), img_corrner_point, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);

		cout << "Found corners: " << img_corrner_point.size() << endl;

		//顯示角點
		if (found_success)
		{
			//跌代終止條件
			TermCriteria criteria(TermCriteria::EPS | TermCriteria::MAX_ITER, 30, 0.001);
			cornerSubPix(img_gray, img_corrner_point, Size(11, 11), Size(-1, -1), criteria);


			//提取亞像素角點//size(11,11)的意思還不太了解
			//cornerSubPix(img_gray, img_corrner_point, Size(11, 11), Size(-1, -1), criteria);
			cout << "Refined corners: " << img_corrner_point.size() << endl;
			//繪製角點
			drawChessboardCorners(image, Size(boardsize[0], boardsize[1]), img_corrner_point, found_success);

			chess_point.push_back(world_point);//從世界座標至相機座標系
			image_point.push_back(img_corrner_point);

		}
		drawChessboardCorners(image, Size(boardsize[0], boardsize[1]), img_corrner_point, found_success);

		imshow("live", image);
		waitKey(500);

	}
	cout << "Size of chess_point: " << chess_point.size() << endl;
	cout << "Size of image_point: " << image_point.size() << endl;
	// Print out some data from the vectors if needed for debugging
	for (size_t i = 0; i < chess_point.size(); ++i) {
		cout << "chess_point[" << i << "]: " << chess_point[i] << endl;
		// Similarly for image_point if applicable
	}
	Mat cameraMatrix, distCoeffs, R, T;//內餐矩陣,畸變係數,旋轉矩陣,偏移矩陣
	calibrateCamera(chess_point, image_point, img_gray.size(), cameraMatrix, distCoeffs, R, T);


	// 顯示校正結果
	cout << "cameraMatrix:" << endl << cameraMatrix << endl;
	cout << "*************************" << endl;
	cout << "distCoeffs:" << endl << distCoeffs << endl;
	cout << "*************************" << endl;
	cout << "Rotation vector:" << endl << R << endl;
	cout << "************************" << endl;
	cout << "Translation vector:" << endl << T << endl;

	// 將相機校正參數保存到 XML 檔案
	FileStorage fs("calibration.xml", FileStorage::WRITE);
	fs << "cameraMatrix" << cameraMatrix;
	fs << "distCoeffs" << distCoeffs;
	fs << "rotationMatrix" << R;
	fs << "translationVector" << T;
	fs.release();


	destroyAllWindows();//取消顯示窗口
	system("pause");


	return 0;

}

int CamImgChessAndCaptureCalibration()
{//相機影像捕捉、棋盤格角點偵測、存圖、相機校正
	VideoCapture cap;

	Mat readImage;
	Mat readImageClone;
	vector<Mat>readImage_list;
	cap.set(CAP_PROP_FRAME_HEIGHT, 1280);
	cap.set(CAP_PROP_FRAME_WIDTH, 720);
	cap.open(0);

	int m=9;
	int n=6;
	//cout << "輸入棋盤格角點" << endl;
	//cin >> m >> n;

	int i;
	i = 1;

	if (!cap.isOpened())
		return -1;

	char x[100] = "D:/visual_practice/practice/Camera calibration/co/cal";//字串創建

	//char y[100] = "C:/Users/User/Desktop/123/測試2/";

	namedWindow("chessboard corners");
	while (true) {
		if (!cap.read(readImage)) {
			cout << "No frame" << endl;
			break;
		}

		readImageClone = readImage.clone();

		vector<Point2f> corners;
		Size PatSize;

		PatSize.height = m;
		PatSize.width = n;

		bool ret = findChessboardCorners(readImageClone, PatSize, corners, CALIB_CB_FAST_CHECK);
		drawChessboardCorners(readImageClone, PatSize, corners, ret);
		imshow("chessboard corners", readImageClone);

		if (waitKey(5) == 32)//SPACE
		{
			if (ret) {
				string image_name = x + to_string(i) + ".png";
				imwrite(image_name, readImage);
				//string image_name2 = y + to_string(i) + ".png";
				//imwrite(image_name2, readImageClone);

				readImage_list.push_back(readImage.clone());
			}
			i++;
		}
		else if (waitKey(5) == 27)break;
		

	}

	Mat img_gray, image;


	int boardsize[2]{ m,n };

	vector<vector<Point3f>>chess_point;//棋盤格角點的三維座標
	vector<Point3f>world_point;//三維世界座標
	vector<vector<Point2f>>image_point;//保存所有角點
	vector<Point2f>img_corrner_point;//保存偵測到的角點
	vector<String>imges_path;//建立物件存放讀取路徑

	string imge_path = "D:/visual_practice/practice/Camera calibration/co/cal/*.png";//待處理圖路徑
	glob(imge_path, imges_path);//讀取指定文件夾圖像

	//轉世界座標
	for (int i = 0; i < boardsize[1]; i++)
	{
		for (int j = 0; j < boardsize[0]; j++)
		{
			world_point.push_back(Point3f(j, i, 0));
		}

	}

	for (int i = 0; i < readImage_list.size(); i++)
	{
		image = readImage_list[i];
		cvtColor(image, img_gray, COLOR_BGR2GRAY);
		//檢測角點
		bool found_success = findChessboardCorners(img_gray, Size(boardsize[0], boardsize[1]), \
			img_corrner_point, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);

		//顯示角點
		if (found_success)
		{
			//跌代終止條件
			TermCriteria criteria(TermCriteria::EPS | TermCriteria::MAX_ITER, 30, 0.001);
			//疊代終止條件類型,最大疊代次數,疊代算法停止的期望精度或參數更改
			//https://www.jianshu.com/p/548868c4d34e
			//https://read01.com/zh-tw/Be5Rd.html#.YaSZp9BByUk



			//提取亞像素角點//size(11,11)的意思還不太了解
			cornerSubPix(img_gray, img_corrner_point, Size(11, 11), Size(-1, -1), criteria);
			//輸入影像,偵測的角點,搜索窗口的一半尺寸,搜索需昱中間的dead region邊長的一半 如果值設為(-1,-1)
			//則表示沒有這個區域
			//https://www.itread01.com/content/1544079906.html
			//https://blog.csdn.net/holybin/article/details/41122493
			//https://www.cnblogs.com/bjxqmy/p/12484834.html


			//繪製角點
			drawChessboardCorners(image, Size(boardsize[0], boardsize[1]), img_corrner_point, found_success);

			chess_point.push_back(world_point);//從世界座標至相機座標系
			image_point.push_back(img_corrner_point);

		}

		imshow("live", image);
		waitKey(200);

	}


	Mat cameraMatrix, distCoeffs, R, T;//內參矩陣,畸變係數,旋轉矩陣,偏移矩陣
	calibrateCamera(chess_point, image_point, img_gray.size(), cameraMatrix, distCoeffs, R, T);
	//世界座標,保存的角點,圖像大小,輸出相機內參矩陣,輸出畸變矩陣,旋轉矩陣,偏移矩陣



	cout << "cameraMatrix:" << endl;//顯示內參矩陣
	cout << cameraMatrix << endl;

	cout << "*************************" << endl;
	cout << "distCoeffs:" << endl;//顯示畸變參數
	cout << distCoeffs << endl;

	cout << "*************************" << endl;
	cout << "Rotation vector:" << endl;//顯示旋轉矩陣
	cout << R << endl;

	cout << "************************" << endl;
	cout << "Translation vector:" << endl;//顯示偏移矩陣
	cout << T << endl;


	//vector<Point2f> reprojectedPoints;
	//vector<Point3f> objectPoints; // 这里存放您的 3D 物体点

	//// 对每个图像进行重投影验证
	//for (int i = 0; i < image_point.size(); i++) {
	//	// 使用 calibrateCamera 得到的参数进行重投影
	//	projectPoints(objectPoints, R, T, cameraMatrix, distCoeffs, reprojectedPoints);

	//	// 计算实际检测到的角点与重投影的误差
	//	double error = norm(image_point[i], reprojectedPoints, NORM_L2);
	//	cout << "Reprojection error for image " << i << ": " << error << endl;
	//}
	//FileStorage fs("camera_calibration.xml", FileStorage::WRITE);

	//if (fs.isOpened()) {
	//	fs << "camera_matrix" << cameraMatrix;
	//	fs << "distortion_coefficients" << distCoeffs;
	//	fs.release();
	//	cout << "Camera calibration data saved to camera_calibration.xml." << endl;
	//}
	//else {
	//	cerr << "Error: Could not open the XML file for writing." << endl;
	//}


	while (true)
	{
		Mat frame;
		Mat undistortedFrame;
		cap >> frame;
		undistort(frame, undistortedFrame, cameraMatrix, distCoeffs);
		if (frame.empty()) {
			break;
		}

		imshow("Original", frame);
		imshow("Undistorted", undistortedFrame);

		if (waitKey(1) == 27) { // Esc退出
			break;
		}
	}

	destroyAllWindows();//取消顯示窗口
	system("pause");

	return 0;
}

//********************************************************

int main() {
	//CamImgChessAndCaptureCalibration();
	ImgCalibration();
	//CamImagCalibrate();

	return 0;
}
