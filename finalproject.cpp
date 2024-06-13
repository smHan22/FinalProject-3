#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

Point pt0ld;
void on_mouse(int event, int x, int y, int flags, void* userdata);
void Clear(const Mat& image);
void Save(const Mat& image);
void Load(const Mat& image);
void Contour(const Mat& image);
void CenterPoint(const Mat& image);
int run(const Mat& image);
tuple<int, int, int, int> PixelRatio(const Mat& img);
tuple<Point, Point, vector<Point>> ShowContoursAndCenters(const Mat& Image);
Mat src(500, 900, CV_8UC3, Scalar(255, 255, 255));
Mat drawLine();



int main() {
	Mat img = drawLine();
	namedWindow("Windows");
	setMouseCallback("Windows", on_mouse, &img);

	imshow("Windows", img);
	waitKey();
	return 0;
}

void on_mouse(int event, int x, int y, int flags, void* userdata) {
	switch (event) {
	case EVENT_LBUTTONDOWN:
		pt0ld = Point(x, y);
		if (Rect(500, 200, 200, 100).contains(Point(x, y))) {       // clear
			Clear((*(Mat*)userdata));
		}
		else if (Rect(500, 0, 200, 100).contains(Point(x, y))) {    // save
			Save((*(Mat*)userdata));
		}
		else if (Rect(500, 100, 200, 100).contains(Point(x, y))) {  // load
			Load((*(Mat*)userdata));
		}
		else if (Rect(500, 400, 200, 100).contains(Point(x, y))) {  // exit
			exit(1);
		}
		else if (Rect(500, 300, 200, 100).contains(Point(x, y))) {  // run
			int num = run(*(Mat*)userdata);

			if (num == -1) {
				cout << "숫자를 인식하지 못했습니다." << endl;
			}
			cout << "결과: " << num << endl;
		}
		else if (Rect(700, 0, 200, 100).contains(Point(x, y))) {    // contour
			Contour((*(Mat*)userdata));
		}
		else if (Rect(700, 100, 200, 100).contains(Point(x, y))) {  // CenterPoint
			CenterPoint((*(Mat*)userdata));
		}
		else if (Rect(700, 200, 200, 100).contains(Point(x, y))) {		// PixelCount
			int u_pixel_count, d_pixel_count, l_pixel_count, r_pixel_count;
			tie(u_pixel_count, d_pixel_count, l_pixel_count, r_pixel_count) = PixelRatio((*(Mat*)userdata));
			cout << "upPixels count: " << u_pixel_count << endl;
			cout << "downPixels count: " << d_pixel_count << endl;
			cout << "leftPixels count: " << l_pixel_count << endl;
			cout << "rightPixels count: " << r_pixel_count << endl;
		}
		break;
	case EVENT_MOUSEMOVE:
		if (Rect(0, 0, 495, 500).contains(Point(x, y))) {
			if (flags & EVENT_FLAG_LBUTTON) {
				line(src, pt0ld, Point(x, y), Scalar(0), 5);
				imshow("Windows", *(Mat*)userdata);
				pt0ld = Point(x, y);
	case EVENT_LBUTTONUP:
		pt0ld = Point(x, y);
		break;
	default:
		break;
			}
		}
	}
}

Mat drawLine() {		// UI 그리기 함수
	rectangle(src, Rect(0, 0, 700, 500), Scalar(0, 0, 0), 2);
	rectangle(src, Rect(500, 0, 200, 100), Scalar(0, 0, 0), 2);     // save block
	rectangle(src, Rect(500, 100, 200, 100), Scalar(0, 0, 0), 2);   // load block
	rectangle(src, Rect(500, 200, 200, 100), Scalar(0, 0, 0), 2);   // clear block
	rectangle(src, Rect(500, 300, 200, 100), Scalar(0, 0, 0), 2);   // run block
	rectangle(src, Rect(500, 400, 200, 100), Scalar(0, 0, 0), 2);   // exit block
	rectangle(src, Rect(700, 0, 200, 100), Scalar(0, 0, 0), 2);     // contours block
	rectangle(src, Rect(700, 100, 200, 100), Scalar(0, 0, 0), 2);   // CenterPoint block
	rectangle(src, Rect(700, 200, 200, 100), Scalar(0, 0, 0), 2);		// PixelRatio block
	rectangle(src, Rect(700, 300, 200, 100), Scalar(0, 0, 0), 2);
	rectangle(src, Rect(700, 400, 200, 100), Scalar(0, 0, 0), 2);

	const string text1[] = { "Save", "Load", "Clear", "Run", "Exit" };
	const string text2[] = { "contour", "CenterPoint", "PixelRatio", " ", " " };
	int fontFace = FONT_HERSHEY_SIMPLEX;
	double fontScale = 0.7;
	int thickness = 2;

	for (int i = 0; i < 5; i++) {
		Size sizeText = getTextSize(text1[i], fontFace, fontScale, thickness, 0);
		Size sizeText1 = getTextSize(text2[i], fontFace, fontScale, thickness, 0);
		Size sizeImg = src.size();
		int buttonXStart1 = 500;
		int buttonXStart2 = 700;
		int buttonWidth = 200;

		Point org1(buttonXStart1 + (buttonWidth - sizeText.width) / 2, 500 * i / 5 + (sizeImg.height + sizeText.height) / 8);
		Point org2(buttonXStart2 + (buttonWidth - sizeText1.width) / 2, 500 * i / 5 + (sizeImg.height + sizeText1.height) / 8);

		putText(src, text1[i], org1, fontFace, fontScale, Scalar(0), thickness);
		putText(src, text2[i], org2, fontFace, fontScale, Scalar(0), thickness);
	}
	return src;
}

// 각 버튼 역할 함수 정의
void Clear(const Mat& image) {		// Clear
	(image)(Rect(0, 0, 500, 500)) = Scalar(255, 255, 255); // userdata를 Mat 타입으로 캐스팅하고 이를 참조하여 500x500 영역을 흰색으로 채움
	rectangle(image, Rect(0, 0, 500, 500), Scalar(0), 2); // 같은 Mat 객체에 검은색 테두리를 그림
	cout << "입력창 삭제됨" << endl;
	imshow("Windows", image); // 변경된 이미지를 다시 표시함
}

void Save(const Mat& image) {		// Save
	string fileName;
	Mat save = (image)(Rect(2, 2, 496, 496)).clone();
	resize(save, save, Size(500, 500));
	cout << "파일명 입력: ";
	cin >> fileName;
	imwrite(fileName, save);
	cout << fileName << "이 저장됨" << endl;
}
void Load(const Mat& image) {		// Load
	string fileName;
	cout << "파일명을 입력하시오: ";
	cin >> fileName;
	Mat numberImg = imread(fileName);
	rectangle(numberImg, Rect(0, 0, 500, 500), Scalar(0), 2);
	numberImg.copyTo((image)(Rect(0, 0, 500, 500)));
	imshow("Windows", image);
}
void Contour(const Mat& image) {		// Contour
	Mat gray, bin;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	Mat gray_img = gray(Rect(2, 2, 496, 496));
	threshold(gray_img, bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
	morphologyEx(bin, bin, MORPH_CLOSE, Mat(30, 30, CV_8UC1));
	vector<vector<Point>> contours;
	findContours(bin, contours, RETR_LIST, CHAIN_APPROX_NONE);
	imshow("bin", bin);

	cout << "검출된 외곽선: " << contours.size() << endl;
	if (contours.size() == 1) {
		cout << "예상 숫자: 1, 2, 3, 4, 5, 7" << endl;
	}
	else if (contours.size() == 2) {
		cout << "예상 숫자: 0, 4, 6, 9" << endl;
	}
	else if (contours.size() == 3) {
		cout << "예상 숫자: 8" << endl;
	}
}
void CenterPoint(const Mat& image) {		// CenterPoint
	Mat gray, bin;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	Mat gray_img = gray(Rect(2, 2, 496, 496));
	threshold(gray_img, bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
	morphologyEx(bin, bin, MORPH_CLOSE, Mat(30, 30, CV_8UC1));
	vector<vector<Point>> contours;
	findContours(bin, contours, RETR_LIST, CHAIN_APPROX_NONE);

	cout << "검출된 외곽선: " << contours.size() << endl;

	cvtColor(bin, bin, COLOR_GRAY2BGR);
	// 바운딩 박스를 그리고 중심 좌표 계산
	vector<Rect> boundingBoxes;
	for (int i = 0; i < contours.size(); i++) {
		Rect boundingBox = boundingRect(contours[i]);
		boundingBoxes.push_back(boundingBox);
		rectangle(bin, boundingBox, Scalar(0, 255, 0), 2);
	}

	// 바운딩 박스들의 중심 좌표 계산
	vector<Point> centers;
	for (size_t i = 0; i < boundingBoxes.size(); i++) {
		int centerX = boundingBoxes[i].x + boundingBoxes[i].width / 2;
		int centerY = boundingBoxes[i].y + boundingBoxes[i].height / 2;
		Point center(centerX, centerY);
		centers.push_back(center);
		circle(bin, center, 5, Scalar(0, 0, 255), -1);
		cout << "중심 좌표: " << center << endl;

		if (centers.size() >= 2) {
			Point center1 = centers[0];
			Point center2 = centers[1];

			cout << "[ " << center2.x - center1.x << " ," << center2.y - center1.y << " ]" << endl;
		}
		/*
		imshow("Bounding Boxes and Centers", bin);
		*/
	}
}

tuple<Point, Point, vector<Point>> ShowContoursAndCenters(const Mat& image) {		// 외부 외곽선, 내부 외곽선 중심 좌표
	Mat gray, bin;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	Mat gray_img = gray(Rect(2, 2, 496, 496));
	threshold(gray_img, bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
	morphologyEx(bin, bin, MORPH_CLOSE, Mat(30, 30, CV_8UC1));
	vector<vector<Point>> contours;
	findContours(bin, contours, RETR_LIST, CHAIN_APPROX_NONE);

	// 바운딩 박스를 그리고 중심 좌표 계산
	vector<Rect> boundingBoxes;
	cvtColor(bin, bin, COLOR_GRAY2BGR);
	for (int i = 0; i < contours.size(); i++) {
		Rect boundingBox = boundingRect(contours[i]);
		boundingBoxes.push_back(boundingBox);
		rectangle(bin, boundingBox, Scalar(0, 255, 0), 2);
	}

	// 바운딩 박스들의 중심 좌표 계산
	vector<Point> centers;
	for (int i = 0; i < boundingBoxes.size(); i++) {
		int centerX = boundingBoxes[i].x + boundingBoxes[i].width / 2;
		int centerY = boundingBoxes[i].y + boundingBoxes[i].height / 2;
		Point center(centerX, centerY);
		centers.push_back(center);
		circle(bin, center, 5, Scalar(0, 0, 255), -1);
	}
	Point center1 = centers[0];
	Point center2 = centers[1];
	/*
	imshow("Windows", image);
	imshow("Bounding Boxes and Centers", bin);  // 새로운 창에 표시
	*/
	return make_tuple(center1, center2, centers);
}

tuple<int, int, int, int> PixelRatio(const Mat& image) {		// PixelRatio
	Mat gray, bin;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	Mat gray_img = gray(Rect(2, 2, 496, 496));
	threshold(gray_img, bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
	morphologyEx(bin, bin, MORPH_CLOSE, Mat(30, 30, CV_8UC1));

	vector<vector<Point>> contours;
	findContours(bin, contours, RETR_LIST, CHAIN_APPROX_NONE);

	Rect rect = boundingRect(contours[0]);
	int center_x = rect.x + rect.width / 2;
	int center_y = rect.y + rect.height / 2;

	int u_pixel_count = 0, d_pixel_count = 0, l_pixel_count = 0, r_pixel_count = 0;
	for (int i = 0; i < bin.rows; i++) {
		for (int j = 0; j < bin.cols; j++) {
			if (bin.at<uchar>(i, j) == 255) {
				if (i < center_y) {
					u_pixel_count++;
				}
				else {
					d_pixel_count++;
				}
				if (j < center_x) {
					l_pixel_count++;
				}
				else {
					r_pixel_count++;
				}
			}
		}
	}
	int total_vertical_pixels = u_pixel_count + d_pixel_count;
	int total_horizontal_pixels = l_pixel_count + r_pixel_count;
	int u_count = (double)u_pixel_count / total_vertical_pixels * 100;
	int d_count = (double)d_pixel_count / total_vertical_pixels * 100;
	int l_count = (double)l_pixel_count / total_horizontal_pixels * 100;
	int r_count = (double)r_pixel_count / total_horizontal_pixels * 100;

	cvtColor(bin, bin, COLOR_GRAY2BGR);
	for (int i = 0; i < contours.size(); i++) {
		Rect rect = boundingRect(contours[i]);
		rectangle(bin, rect, Scalar(0, 255, 0), 2);
	}

	Rect rect1 = boundingRect(contours[0]);
	Point center(rect1.x + rect1.width / 2, rect1.y + rect1.height / 2);
	circle(bin, center, 5, Scalar(0, 0, 255), -1);

	Point StartPoint = Point(rect1.x + rect1.width / 2, rect1.y);
	Point EndPoint = Point(rect1.x + rect1.width / 2, rect1.y + rect1.height);
	line(bin, StartPoint, EndPoint, Scalar(255, 0, 0), 2);

	Point StartPoint1(rect1.x, rect1.y + rect1.height / 2);
	Point EndPoint1(rect1.x + rect1.width, rect1.y + rect1.height / 2);
	line(bin, StartPoint1, EndPoint1, Scalar(255, 0, 0), 2);

	/*
	imshow("boundingbox", bin);
	*/

	return make_tuple(u_count, d_count, l_count, r_count);
}

int run(const Mat& image) {
	int center1, center2, center3, center4;
	tie(center1, center2, center3, center4) = PixelRatio(image);

	Mat gray, bin;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	Mat gray_img = gray(Rect(2, 2, 496, 496));
	threshold(gray_img, bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
	morphologyEx(bin, bin, MORPH_CLOSE, Mat(30, 30, CV_8UC1));

	vector<vector<Point>> contours;
	findContours(bin, contours, RETR_LIST, CHAIN_APPROX_NONE);

	vector<Rect> boundingBoxes;
	cvtColor(bin, bin, COLOR_GRAY2BGR);
	for (int i = 0; i < contours.size(); i++) {
		Rect boundingBox = boundingRect(contours[i]);
		boundingBoxes.push_back(boundingBox);
	}

	// 바운딩 박스들의 중심 좌표 계산
	vector<Point> centers;
	for (int i = 0; i < boundingBoxes.size(); i++) {
		int centerX = boundingBoxes[i].x + boundingBoxes[i].width / 2;
		int centerY = boundingBoxes[i].y + boundingBoxes[i].height / 2;
		Point center(centerX, centerY);
		centers.push_back(center);
	}

	if (contours.size() == 2) {
		Point Center1 = centers[0];
		Point Center2 = centers[1];
		if (abs(Center1.y - Center2.y) < 10) return 0;
		else if (Center1.y < Center2.y && Center2.y - Center1.y > 60) return 9;
		else if (Center1.y > Center2.y) return 6;
	}
	if (center1 <= 40 && center2 >= 55 && center3 <= 40 && center4 >= 60) return 4;
	if (contours.size() == 3) return 8;
	if (center1 <= 50 && center2 <= 51 && center3 <= 70 && center4 <= 70) return 1;
	else if (center1 <= 47 && center2 >= 55 && center3 >= 50 && center4 <= 60) return 2;
	else if (center1 >= 55 && center2 <= 47 && center3 >= 47 && center4 <= 50) return 5;
	else if (center1 <= 67 && center2 <= 60 && center3 < center4) return 3;
	else if (center1 >= 60 && center2 <= 40 && center3 <= 40 && center4 >= 60) return 7;

	return -1;
}
