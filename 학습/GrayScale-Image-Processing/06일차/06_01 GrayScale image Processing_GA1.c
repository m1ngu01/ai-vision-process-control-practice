/*
이동시키기
dx, dy값 입력 받고 이동시키기
좌우 대칭 상하 대칭
심화 회전 확대
히스토그램 화소점 처리 기능 추가
화소 영역 처리 기능 추가.
*/


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h> // GUI 기능 지원
#include <math.h>

///////////////////            
// 전역 변수 선언부
///////////////////
HWND  hwnd; //  윈도 화면(외부, 물리적)
HDC hdc; // 윈도 화면(내부, 논리적)


int oriH, oriW, tarH, tarW;
unsigned char** oriImage = NULL;
unsigned char** tarImage = NULL;
unsigned char** circle = NULL;

char filename[200];  // 파일명

///////////////////
// 함수 선언부
///////////////////
// 공통 함수부
void loadImage();
void saveImage();
void displayImage();
void printMenu();
void mallocOriImage();
void mallocTarImage();
void freeOriImage();
void freeTarImage();
// 영상처리 함수부
void equalImage();
void addImage();
void reverseImage();
void zoomOut();
void zoomIn();
void zoomIn2();
void rotate();
void rotate2();
void rotate3();
void movement();
void histoStretch();
void endIn();
void histoEqual();
void symmetry();
void verticalSymmetry();
void emboss();
void blurr();
void gaussian();
void shaping();
void edge();
void edge_Homogeneity();
void edge_char();
void Laplacian();
void draw_circle(int oriH, int oriW, int r);
void free_draw_circle();
void orImage();
void andImage();
void xorImage();
void notImage();
void posterizingImage();
void parabolicImage();
void binarizationImage();
void binarizationHalf();
void binarizationAverage();
int compare(const void* a, const void*);
void binarizationCenter();
void zoomIn3();
void gammaImage();



void printMenu() {
	puts("\n GrayScale Image Processing (RC 1)");

	puts("\n [기본 기능]\t\t[화소 점 처리]");
	puts("\n 1. 열기\t\tK. 동일영상");
	puts("\n 2. 저장\t\tL. 밝게/어둡게");
	puts("\n esc. 종료\t\tM. 반전");
	puts("\n [기하학적 처리]\tN. orImage()");
	puts("\n 3. 좌/우 대칭\t\tO. andImage()");
	puts("\n 4. 상/하 대칭\t\tP. xorImage()");
	puts("\n 5. 축소\t\tQ. notImage()");
	puts("\n 6. 확대(포워딩)\tR. posterizingImage() / 포스터라이징");
	puts("\n 7. 확대(백워딩)\tS. parabolicImage()");
	puts("\n 8. 회전\t\tT. binarizationImage() / 이진화");
	puts("\n 9. 회전(수정)\t\tU. 감마 보정");
	puts("\n A. 이동\t\tV. histoStretch");
	puts("\n [화소 영역 처리]\tW. and - in");
	puts("\n B. emboss / 엠보싱 효과\tX. histoEqual");
	puts("\n C. blurr / 블러링");
	puts("\n D. gaussian / 가우시안 스무딩 필터링 처리");
	puts("\n E. shaping / 샤프닝");
	puts("\n F. edge / 경계선 처리");
	puts("\n G. edge_Homogeneity()");
	puts("\n H. edge_char()");
	puts("\n I. Laplacian()");
	puts("\n J. 양선형 보간법()");
}



///////////////////
// 메인 코드부
///////////////////
void main() {
	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(hwnd);  // 윈도10용.  윈도11(hwnd--> NULL)
	system("mode con:cols=200 lines=200");
	char key = 0;
	while (key != 27) { // 27 is the ASCII code for the ESC key
		printMenu();
		key = _getch();
		system("cls");
		switch (key) {
		case '1': loadImage(); break;
		case '2': saveImage(); break;
		case '3': symmetry(); break;
		case '4': verticalSymmetry(); break;
		case '5': zoomOut(); break;
		case '6': zoomIn(); break;
		case '7': zoomIn2(); break;
		case '8': rotate(); break;
		case '9': rotate2(); break;
		case 'A': case 'a': movement(); break;
		case 'B': case 'b': emboss(); break;
		case 'C': case 'c': blurr(); break;
		case 'D': case 'd': gaussian(); break;
		case 'E': case 'e': shaping(); break;
		case 'F': case 'f': edge(); break;
		case 'G': case 'g': edge_Homogeneity(); break;
		case 'H': case 'h': edge_char(); break;
		case 'I': case 'i': Laplacian(); break;
		case 'J': case 'j': zoomIn3(); break;
		case 'K': case 'k': equalImage(); break;
		case 'L': case 'l': Laplacian(); break;
		case 'M': case 'm': reverseImage(); break;
		case 'N': case 'n': orImage(); break;
		case 'O': case 'o': andImage(); break;
		case 'P': case 'p': xorImage(); break;
		case 'Q': case 'q': notImage(); break;
		case 'R': case 'r': posterizingImage(); break;
		case 'S': case 's': parabolicImage(); break;
		case 'T': case 't': binarizationImage(); break;
		case 'U': case 'u': gammaImage(); break;
		case 'V': case 'v': histoStretch(); break;
		case 'W': case 'w': endIn(); break;
		case 'X': case 'x': histoEqual(); break;
		case 27: freeOriImage(); freeTarImage(); break; // 27 is the ESC key
		}
	}
	puts("'ESC 입력감지 프로그램을 종료합니다.'");
}

////////////////
/// 함수 정의부
////////////////
void mallocOriImage() {
	oriImage = (unsigned char**)malloc(sizeof(unsigned char*) * oriH);
	for (int i = 0; i < oriH; i++)
		oriImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * oriW);
}
void mallocTarImage() {
	tarImage = (unsigned char**)malloc(sizeof(unsigned char*) * tarH);
	for (int i = 0; i < tarH; i++)
		tarImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * tarW);
}
void freeOriImage() {
	if (oriImage == NULL)
		return;
	for (int i = 0; i < oriH; i++)
		free(oriImage[i]);
	free(oriImage);
	oriImage = NULL;
}
void freeTarImage() {
	if (tarImage == NULL)
		return;
	for (int i = 0; i < tarH; i++)
		free(tarImage[i]);
	free(tarImage);
	tarImage = NULL;
}
double** malloc_double(int h, int w) {
	double** memory;
	memory = (double**)malloc(sizeof(double*) * h);
	for (int i = 0; i < h; i++)
		memory[i] = (double*)malloc(sizeof(double) * w);
	return memory;
}
void free_double(double** memory, int h) {
	if (memory == NULL)
		return;
	for (int i = 0; i < h; i++)
		free(memory[i]);
	free(memory);
	memory = NULL;
}
void loadImage() {
	char fullname[200] = "C:/Users/User/Desktop/RAW/Etc_Raw(squre)/";
	char tmpName[50];
	printf("파일명-->"); scanf("%s", tmpName);  // cat01, dog05, etc11
	strcat(fullname, tmpName);
	strcat(fullname, ".raw");
	strcpy(filename, fullname);

	//(중요!) 원본영상의 크기를 파악!
	FILE* rfp;
	rfp = fopen(filename, "rb");
	if (!rfp) {
		printf("파일명 틀렸음~ \n");
		return;
	}
	fseek(rfp, 0L, SEEK_END); // 파일의 끝으로 이동
	long long fsize = ftell(rfp);
	fclose(rfp);
	// 원본 영상 크기
	oriH = oriW = sqrt(fsize);
	// 2차원 메모리 할당
	freeOriImage();
	mallocOriImage();

	// 파일 --> 메모리
	rfp = fopen(filename, "rb");
	for (int i = 0; i < oriH; i++)
		fread(oriImage[i], sizeof(unsigned char), oriW, rfp);
	fclose(rfp);
	equalImage();

}
void saveImage() {
	char fullname[200] = "C:/Users/User/Desktop/RAW/Etc_Raw(squre)/";
	char tmpName[50];
	printf("저장 파일명-->"); scanf("%s", tmpName);  // out01, out02 ....
	strcat(fullname, tmpName);
	strcat(fullname, "_512.raw");
	strcpy(filename, fullname);

	FILE* wfp;
	wfp = fopen(filename, "wb");
	// 메모리 >> 파일
	for (int i = 0; i < tarH; i++) {
		fwrite(tarImage[i], sizeof(unsigned char), tarW, wfp);
	}
	fclose(wfp);
	printf("%s 로 저장됨", filename);
}
void displayImage() {
	system("CLS");
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			int px = tarImage[i][k];
			SetPixel(hdc, k + 700, i + 100, RGB(px, px, px));
		}
	}
}

/// <summary>
/// 영상 처리
/// </summary>
void equalImage() {
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	freeTarImage();
	mallocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tarImage[i][k] = oriImage[i][k];
		}
	}
	///////////////////////////////
	displayImage();
}
void addImage() {
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	freeTarImage();
	mallocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	int value;
	printf("더할 값(-255~+255) :");  scanf("%d", &value);
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			int pixel = oriImage[i][k] + value;
			if (pixel > 255)
				pixel = 255;
			if (pixel < 0)
				pixel = 0;
			tarImage[i][k] = (unsigned char)pixel;
		}
	}
	///////////////////////////////
	displayImage();
}
void reverseImage() {  // 반전 : 0>255, 1>254, 2>253..... 254>1, 255>0
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	freeTarImage();
	mallocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tarImage[i][k] = 255 - oriImage[i][k];
		}
	}
	///////////////////////////////
	displayImage();
}
void zoomOut() {
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	int scale;
	printf("축소 배율:"); scanf("%d", &scale);
	tarH = (int)(oriH / scale);
	tarW = (int)(oriW / scale);
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	freeTarImage();
	mallocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tarImage[i / scale][k / scale] = oriImage[i][k];
		}
	}
	///////////////////////////////
	displayImage();
}
void zoomIn() {
	freeTarImage();
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	int scale;
	printf("확대 배율:"); scanf("%d", &scale);
	tarH = (int)(oriH * scale);
	tarW = (int)(oriW * scale);
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당	
	mallocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tarImage[i * scale][k * scale] = oriImage[i][k];
		}
	}
	///////////////////////////////
	displayImage();
}
void zoomIn2() {
	freeTarImage();
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	int scale;
	printf("확대 배율:"); scanf("%d", &scale);
	tarH = (int)(oriH * scale);
	tarW = (int)(oriW * scale);
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당	
	mallocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			tarImage[i][k] = oriImage[i / scale][k / scale];
		}
	}
	///////////////////////////////
	displayImage();
}
void rotate() {

	int angle;
	int tarX, tarY, oriX, oriY;
	double radian;

	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	freeTarImage();
	mallocTarImage();

	// 회전 각도 입력 받기
	printf("회전 각도: ");
	scanf("%d", &angle);
	// target x = cos * orix - sin * oriy
	// target y = sin * orix + cos * oriy
	radian = angle * 3.141592 / 180.0;

	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			oriX = i;
			oriY = k;
			tarX = cos(radian) * oriX - sin(radian) * oriY;
			tarY = sin(radian) * oriX + cos(radian) * oriY;

			if (tarX >= 0 && tarX < tarH && 0 <= tarY && tarY < tarW)
				tarImage[tarX][tarY] = oriImage[oriX][oriY];
		}
	}
	///////////////////////////////
	displayImage();
}
void rotate2() {
	freeTarImage();
	int angle;
	printf("회전 각도: ");
	scanf("%d", &angle);

	// 각도를 라디안으로 변환
	double radian = angle * 3.141592 / 180.0;

	// 회전 전후 중심점
	int Cx = oriW / 2;
	int Cy = oriH / 2;

	// 회전된 이미지의 크기 계산
	tarW = abs(oriW * cos(radian)) + abs(oriH * sin(radian));
	tarH = abs(oriW * sin(radian)) + abs(oriH * cos(radian));

	// 결과 영상 메모리 할당
	mallocTarImage();

	int tarX, tarY, oriX, oriY;
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			// 회전 후 좌표
			tarX = k;
			tarY = i;

			// 임시 input 사이즈 증가
			// 역회전 변환
			oriX = cos(radian) * (tarX - tarW / 2) + sin(radian) * (tarY - tarH / 2) + Cx;
			oriY = -sin(radian) * (tarX - tarW / 2) + cos(radian) * (tarY - tarH / 2) + Cy;

			// 원본 이미지 범위 내에 있는지 확인
			if ((0 <= oriX && oriX < oriW) && (0 <= oriY && oriY < oriH)) {
				tarImage[tarY][tarX] = oriImage[oriY][oriX];
			}
			else {
				tarImage[tarY][tarX] = 255; // 범위를 벗어나는 경우 검정색으로 채움
			}
		}
	}

	// 이미지 출력 함수
	displayImage();
}
void rotate3() {
	freeTarImage();
	int angle;
	int tarX, tarY, oriX, oriY;
	int Cx, Cy;
	double radian;

	// 회전 각도 입력 받기
	printf("회전 각도: ");
	scanf("%d", &angle);

	// 원본 이미지 중심 좌표
	/*Cx = oriW / 2;
	Cy = oriH / 2;*/
	radian = angle * 3.141592 / 180.0; // 각도를 라디안으로 변환

	// 결과 영상의 크기 결정
	/*tarW = oriW;
	tarH = oriH;*/
	tarW = (int)(oriH * (cos(90 - radian)) + oriW * (cos(radian)));
	tarH = (int)(oriH * (cos(radian)) + oriW * (cos(90 - radian)));

	Cx = tarW / 2;
	Cy = tarH / 2;

	// 결과 영상 메모리 할당
	mallocTarImage();

	// 회전 변환
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			// 결과 이미지에서의 좌표
			tarX = k;
			tarY = i;

			// 원본 이미지에서의 좌표 계산
			oriX = (int)(cos(radian) * (tarX - Cx) + sin(radian) * (tarY - Cy) + Cx);
			oriY = (int)(-sin(radian) * (tarX - Cx) + cos(radian) * (tarY - Cy) + Cy);

			// 좌표가 원본 이미지 범위 내에 있는지 확인
			if (oriX >= 0 && oriX < oriW && oriY >= 0 && oriY < oriH) {
				tarImage[tarY][tarX] = oriImage[oriY][oriX];
			}
			else {
				tarImage[tarY][tarX] = 255; // 범위를 벗어난 경우 빈 값으로 설정
			}
		}
	}

	displayImage();
}
void movement() {
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	freeTarImage();
	mallocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	int dx, dy;
	int mx, my;
	printf("이동 x : ");  scanf("%d", &dx);
	printf("이동 y : ");  scanf("%d", &dy);


	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			mx = dx + k;
			my = dy + i;
			if ((mx >= 0 && mx < oriH) && (my >= 0 && my < oriW)) {
				tarImage[my][mx] = oriImage[i][k];
			}

		}
	}
	///////////////////////////////
	displayImage();
}
void histoStretch() {
	/* 변수*/
	int low, high;


	/* init */
	// 수식 : new = (old - low ) / (high - low) * 255.0
	low = high = oriImage[0][0];
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			if (oriImage[i][k] < low) {
				low = oriImage[i][k];
			}
			if (oriImage[i][k] > high) {
				high = oriImage[i][k];
			}
		}
	}

	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존
	tarH = oriH;
	tarW = oriW;
	freeTarImage();
	mallocTarImage();

	/* algo*/
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tarImage[i][k] = (double)(oriImage[i][k] - low) / (high - low) * 255.0;
		}
	}

	displayImage();
}
void endIn() {
	/* 변수*/
	int low, high;


	/* init */
	// 수식 : new = (old - low ) / (high - low) * 255.0
	low = high = oriImage[0][0];
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			if (oriImage[i][k] < low) {
				low = oriImage[i][k];
			}
			if (oriImage[i][k] > high) {
				high = oriImage[i][k];
			}
		}
	}

	// and - in 알고리즘 적용
	low = low + 50;
	high = high + 50;

	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존
	tarH = oriH;
	tarW = oriW;
	freeTarImage();
	mallocTarImage();

	/* algo*/
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			double newVal = (double)(oriImage[i][k] - low) /
				(high - low) * 255.0;
			if (newVal < 0.0)
				newVal = 0;
			if (newVal > 255.0)
				newVal = 255;

			tarImage[i][k] = newVal;
		}
	}

	displayImage();
}
void histoEqual() {
	/* 변수 */
	int brightness[256] = { 0 };
	double tarBrightness[256] = { 0.0 };
	int brightnessSum[256] = { 0 };

	/* init */
	tarH = oriH;
	tarW = oriW;
	freeTarImage();
	mallocTarImage();

	/* logic */
	// 1단계
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			brightness[oriImage[i][k]]++;
		}
	}

	// 2단계
	brightnessSum[0] = brightness[0];
	for (int i = 1; i < 256; i++) {
		brightnessSum[i] = brightnessSum[i - 1] + brightness[i];
	}

	// 3단계
	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < 256; i++) {
		tarBrightness[i] = brightnessSum[i] * (1.0 / (oriH * oriW)) * 255.0;
	}

	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tarImage[i][k] = (unsigned char)tarBrightness[oriImage[i][k]];
		}
	}

	displayImage();
}
void symmetry() {

	int angle;
	int tarX, tarY, oriX, oriY;

	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	freeTarImage();
	mallocTarImage();

	// 회전 각도 입력 받기
	/*printf("회전 각도: ");
	scanf("%d", &angle);*/
	// oriX = cos * (tarX - Cx) + sin * (tarY - Cy) + Cx
	// oriY = -sin * (tarX - Cx) + cos * (tarY - Cy) + Cy
	// target y = sin * orix + cos * oriy
	//Cx = oriW / 2;
	//radian = 180 * 3.141592 / 180.0;

	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			tarY = i;
			tarX = k;
			tarImage[tarY][tarX] = oriImage[tarY][tarW - tarX];
		}
	}
	///////////////////////////////
	displayImage();
}
void verticalSymmetry() {
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;

	// 결과 영상 메모리 할당
	freeTarImage();
	mallocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			int mirroredY = oriH - 1 - i; // Flip vertically

			if (mirroredY >= 0 && mirroredY < tarH) {
				tarImage[mirroredY][k] = oriImage[i][k];
			}
		}
	}
	///////////////////////////////
	displayImage();
}
void emboss() {
	freeTarImage();
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	mallocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	// const int MSIZE = 3; // 마스크 크기
	// 중요! 마스크 준비
#define MSIZE 3   
	double mask[MSIZE][MSIZE] = {
		{  -1.0, 0.0,  0.0 },
		{  0.0, 0.0,  0.0 },
		{  0.0, 0.0, 1.0 } };
	// 임시 입출력 메모리 준비
	double** tmpOriImage, ** tmpTarImage;
	tmpOriImage = malloc_double(oriH + 2, oriW + 2);
	tmpTarImage = malloc_double(tarH, tarW);
	// 임시 우너본 이미지에 127채우기 평균값이 더 나을듯
	for (int i = 0; i < oriH + 2; i++) {
		for (int k = 0; k < oriW + 2; k++) {
			tmpOriImage[i][k] = 127.0;
		}
	}
	// 원본 --> 임시 원본
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tmpOriImage[i + 1][k + 1] = oriImage[i][k];
		}
	}
	// 회선 연산 --> 마스크로 긁어가면서 연산
	double S = 0; // 각점에 대한 마스크 연산 합계
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			S = 0.0; // 누적 초기화
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += mask[m][n] * tmpOriImage[i + m][k + n];
			tmpTarImage[i][k] = S;
		}
	}
	// Post Processing(후 처리). 마스크 합계가 0이면 127정도를 더함
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			tmpTarImage[i][k] += 127.0;
		}
	}
	// 임시 타켓 --> 타켓으로
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			double v = tmpTarImage[i][k];

			if (v < 0.0) {
				v = 0.0;
			}
			if (v > 255.0) {
				v = 255.0;
			}
			tarImage[i][k] = v;
		}
	}

	free_double(tmpOriImage, oriH + 2);
	free_double(tmpTarImage, tarH);
	///////////////////////////////
	displayImage();
}
void blurr() {
	freeTarImage();
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	mallocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	// const int MSIZE = 3; // 마스크 크기
	// 중요! 마스크 준비
#define MSIZE 3   
	double mask[MSIZE][MSIZE] = {
		{  1.0 / 9, 1.0 / 9,  1.0 / 9 },
		{  1.0 / 9, 1.0 / 9,  1.0 / 9 },
		{  1.0 / 9, 1.0 / 9,  1.0 / 9 } };
	// 임시 입출력 메모리 준비
	double** tmpOriImage, ** tmpTarImage;
	tmpOriImage = malloc_double(oriH + 2, oriW + 2);
	tmpTarImage = malloc_double(tarH, tarW);
	// 임시 우너본 이미지에 127채우기 평균값이 더 나을듯
	for (int i = 0; i < oriH + 2; i++) {
		for (int k = 0; k < oriW + 2; k++) {
			tmpOriImage[i][k] = 127.0;
		}
	}
	// 원본 --> 임시 원본
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tmpOriImage[i + 1][k + 1] = oriImage[i][k];
		}
	}
	// 회선 연산 --> 마스크로 긁어가면서 연산
	double S = 0; // 각점에 대한 마스크 연산의 합계

	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			S = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					S += mask[m][n] * tmpOriImage[i + m][k + n];
				}
			}
			tmpTarImage[i][k] = S;
		}
	}
	// Post Processing(후 처리). 마스크 합계가 0이면 127정도를 더함
	/*for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			tmpTarImage[i][k] += 127.0;
		}
	}*/
	// 임시 타켓 --> 타켓으로
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			double v = tmpTarImage[i][k];

			if (v < 0.0) {
				v = 0.0;
			}
			if (v > 255.0) {
				v = 255.0;
			}
			tarImage[i][k] = v;
		}
	}

	free_double(tmpOriImage, oriH + 2);
	free_double(tmpTarImage, tarH);
	///////////////////////////////
	displayImage();
}
void gaussian() {
	freeTarImage();
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	mallocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	// const int MSIZE = 3; // 마스크 크기
	// 중요! 마스크 준비
#define MSIZE 3   
	double mask[MSIZE][MSIZE] = {
		{  1.0 / 16.0, 1.0 / 8.0,  1.0 / 16.0 },
		{  1.0 / 8.0, 1.0 / 4.0,  1.0 / 8.0 },
		{  1.0 / 16.0, 1.0 / 8.0,  1.0 / 16.0 } };


	// 임시 입출력 메모리 준비
	double** tmpOriImage, ** tmpTarImage;
	tmpOriImage = malloc_double(oriH + 2, oriW + 2);
	tmpTarImage = malloc_double(tarH, tarW);
	// 임시 우너본 이미지에 127채우기 평균값이 더 나을듯
	for (int i = 0; i < oriH + 2; i++) {
		for (int k = 0; k < oriW + 2; k++) {
			tmpOriImage[i][k] = 127.0;
		}
	}
	// 원본 --> 임시 원본
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tmpOriImage[i + 1][k + 1] = oriImage[i][k];
		}
	}
	// 회선 연산 --> 마스크로 긁어가면서 연산
	double S = 0; // 각점에 대한 마스크 연산의 합계

	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			S = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					S += mask[m][n] * tmpOriImage[i + m][k + n];
				}
			}
			tmpTarImage[i][k] = S;
		}
	}
	// Post Processing(후 처리). 마스크 합계가 0이면 127정도를 더함
	/*for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			tmpTarImage[i][k] += 127.0;
		}
	}*/
	// 임시 타켓 --> 타켓으로
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			double v = tmpTarImage[i][k];

			if (v < 0.0) {
				v = 0.0;
			}
			if (v > 255.0) {
				v = 255.0;
			}
			tarImage[i][k] = v;
		}
	}

	free_double(tmpOriImage, oriH + 2);
	free_double(tmpTarImage, tarH);
	///////////////////////////////
	displayImage();
}
void shaping() {
	freeTarImage();
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	mallocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	// const int MSIZE = 3; // 마스크 크기
	// 중요! 마스크 준비
#define MSIZE 3   
	//double mask[MSIZE][MSIZE] = {
	//	{  -1.0, -1.0,  -1.0 },
	//	{  -1.0, 9.0,  -1.0 },
	//	{  -1.0, -1.0, -1.0 } };
	double mask[MSIZE][MSIZE] = {
		{  0.0, -1.0,  0.0 },
		{  -1.0, 5.0,  -1.0 },
		{  0.0, -1.0, 0.0 } };


	// 임시 입출력 메모리 준비
	double** tmpOriImage, ** tmpTarImage;
	tmpOriImage = malloc_double(oriH + 2, oriW + 2);
	tmpTarImage = malloc_double(tarH, tarW);
	// 임시 우너본 이미지에 127채우기 평균값이 더 나을듯
	for (int i = 0; i < oriH + 2; i++) {
		for (int k = 0; k < oriW + 2; k++) {
			tmpOriImage[i][k] = 127.0;
		}
	}
	// 원본 --> 임시 원본
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tmpOriImage[i + 1][k + 1] = oriImage[i][k];
		}
	}
	// 회선 연산 --> 마스크로 긁어가면서 연산
	double S = 0; // 각점에 대한 마스크 연산의 합계

	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			S = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					S += mask[m][n] * tmpOriImage[i + m][k + n];
				}
			}
			tmpTarImage[i][k] = S;
		}
	}
	// Post Processing(후 처리). 마스크 합계가 0이면 127정도를 더함
	/*for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			tmpTarImage[i][k] += 127.0;
		}
	}*/
	// 임시 타켓 --> 타켓으로
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			double v = tmpTarImage[i][k];

			if (v < 0.0) {
				v = 0.0;
			}
			if (v > 255.0) {
				v = 255.0;
			}
			tarImage[i][k] = v;
		}
	}

	free_double(tmpOriImage, oriH + 2);
	free_double(tmpTarImage, tarH);
	///////////////////////////////
	displayImage();
}
void edge() {
	freeTarImage();
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	mallocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	// const int MSIZE = 3; // 마스크 크기
	// 중요! 마스크 준비
#define MSIZE 3   
	double mask[MSIZE][MSIZE] = {
		{  0.0, 0.0,  0.0 },
		{ -1.0, 1.0,  0.0 },
		{  0.0, 0.0,  0.0 } };
	// 임시 입출력 메모리 준비
	double** tmpOriImage, ** tmpTarImage;
	tmpOriImage = malloc_double(oriH + 2, oriW + 2);
	tmpTarImage = malloc_double(tarH, tarW);
	// 임시 우너본 이미지에 127채우기 평균값이 더 나을듯
	for (int i = 0; i < oriH + 2; i++) {
		for (int k = 0; k < oriW + 2; k++) {
			tmpOriImage[i][k] = 127.0;
		}
	}
	// 원본 --> 임시 원본
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tmpOriImage[i + 1][k + 1] = oriImage[i][k];
		}
	}
	// 회선 연산 --> 마스크로 긁어가면서 연산
	double S = 0; // 각점에 대한 마스크 연산의 합계

	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			S = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					S += mask[m][n] * tmpOriImage[i + m][k + n];
				}
			}
			tmpTarImage[i][k] = S + 50;
		}
	}
	// Post Processing(후 처리). 마스크 합계가 0이면 127정도를 더함
	/*for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			tmpTarImage[i][k] += 127.0;
		}
	}*/
	// 임시 타켓 --> 타켓으로
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			double v = tmpTarImage[i][k];

			if (v < 0.0) {
				v = 0.0;
			}
			if (v > 255.0) {
				v = 255.0;
			}
			tarImage[i][k] = v;
		}
	}

	free_double(tmpOriImage, oriH + 2);
	free_double(tmpTarImage, tarH);
	///////////////////////////////
	displayImage();
}
void edge_Homogeneity() {
	freeTarImage();
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	mallocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	// const int MSIZE = 3; // 마스크 크기
	// 중요! 마스크 준비
#define MSIZE 3 
	// 임시 입출력 메모리 준비
	double** tmpOriImage, ** tmpTarImage;
	tmpOriImage = malloc_double(oriH + 2, oriW + 2);
	tmpTarImage = malloc_double(tarH, tarW);
	// 원본 이미지 평균 화소
	double average = 0.0;
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			average = average + oriImage[i][k];
		}
	}
	average = average / (oriH * oriW);

	// 임시 우너본 이미지에 127채우기 평균값이 더 나을듯
	for (int i = 0; i < oriH + 2; i++) {
		for (int k = 0; k < oriW + 2; k++) {
			tmpOriImage[i][k] = average;
		}
	}
	// 원본 --> 임시 원본
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tmpOriImage[i + 1][k + 1] = oriImage[i][k];
		}
	}


	// 회선 연산 --> 마스크로 긁어가면서 연산
	double S = 0.0; // 각점에 대한 마스크 연산의 합계
	double tmp = 0.0;
	// 반복문의 기준은 tarH, tarW /  배열[-1][-1] 오류 방지를 위함
	for (int i = 1; i < tarH - 1; i++) {
		for (int k = 1; k < tarW - 1; k++) {
			S = 0.0;
			for (int m = i - 1; m <= i + 1; m++) {
				for (int n = k - 1; n <= k + 1; n++) {
					tmp = abs(tmpOriImage[i][k] - tmpOriImage[m][n]);
					if (S < tmp) {
						S = tmp;
					}
				}
			}
			tmpTarImage[i][k] = S + 100;
		}
	}

	// Post Processing(후 처리). 마스크 합계가 0이면 127정도를 더함
	/*for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			tmpTarImage[i][k] += 127.0;
		}
	}*/
	// 임시 타켓 --> 타켓으로
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			double v = tmpTarImage[i][k];

			if (v < 0.0) {
				v = 0.0;
			}
			if (v < 110.0) {
				v = 0;
			}
			if (v > 255.0) {
				v = 255.0;
			}
			if (v > 200.0) {
				v = 255.0;
			}
			tarImage[i][k] = v;
		}
	}

	free_double(tmpOriImage, oriH + 2);
	free_double(tmpTarImage, tarH);
	///////////////////////////////
	displayImage();
}
void edge_char() {
	freeTarImage();
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	mallocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	// const int MSIZE = 3; // 마스크 크기
	// 중요! 마스크 준비
#define MSIZE 3 
	// 임시 입출력 메모리 준비
	double** tmpOriImage, ** tmpTarImage;
	tmpOriImage = malloc_double(oriH + 2, oriW + 2);
	tmpTarImage = malloc_double(tarH, tarW);
	// 원본 이미지 평균 화소
	double average = 0.0;
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			average = average + oriImage[i][k];
		}
	}
	average = average / (oriH * oriW);

	// 임시 우너본 이미지에 127채우기 평균값이 더 나을듯
	for (int i = 0; i < oriH + 2; i++) {
		for (int k = 0; k < oriW + 2; k++) {
			tmpOriImage[i][k] = average;
		}
	}
	// 원본 --> 임시 원본
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tmpOriImage[i + 1][k + 1] = oriImage[i][k];
		}
	}


	// 회선 연산 --> 마스크로 긁어가면서 연산
	double S = 0.0; // 각점에 대한 마스크 연산의 합계
	double tmp = 0.0;
	// 반복문의 기준은 tarH, tarW /  배열[-1][-1] 오류 방지를 위함
	for (int i = 1; i < tarH - 1; i++) {
		for (int k = 1; k < tarW - 1; k++) {
			double maxDiff = 0.0;
			double centerPixel = tmpOriImage[i][k];
			for (int m = i - 1; m <= i + 1; m++) {
				for (int n = k - 1; n <= k + 1; n++) {
					if (m == i && n == k) continue; // 중앙 픽셀은 제외
					double diff = abs(centerPixel - tmpOriImage[m][n]);
					if (diff > maxDiff) {
						maxDiff = diff;
					}
				}
			}
			tmpTarImage[i][k] = maxDiff + 100;
		}
	}

	// Post Processing(후 처리). 마스크 합계가 0이면 127정도를 더함
	/*for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			tmpTarImage[i][k] += 127.0;
		}
	}*/
	// 임시 타켓 --> 타켓으로
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			double v = tmpTarImage[i][k];

			if (v < 0.0) {
				v = 0.0;
			}
			if (v < 110.0) {
				v = 0;
			}
			if (v > 255.0) {
				v = 255.0;
			}
			if (v > 200.0) {
				v = 255.0;
			}
			tarImage[i][k] = v;
		}
	}

	free_double(tmpOriImage, oriH + 2);
	free_double(tmpTarImage, tarH);
	///////////////////////////////
	displayImage();
}
void Laplacian() {
	freeTarImage();
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	mallocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	// const int MSIZE = 3; // 마스크 크기
	// 중요! 마스크 준비
#define MSIZE 3   
	double mask[MSIZE][MSIZE] = {
		{  -1.0,-1.0,  -1.0 },
		{  -1.0, 8.0,  -1.0 },
		{  -1.0, -1.0, -1.0 } };
	// 임시 입출력 메모리 준비
	double** tmpOriImage, ** tmpTarImage;
	tmpOriImage = malloc_double(oriH + 2, oriW + 2);
	tmpTarImage = malloc_double(tarH, tarW);
	// 원본 이미지 평균 화소
	double average = 0.0;
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			average = average + oriImage[i][k];
		}
	}
	average = average / (oriH * oriW);

	// 임시 우너본 이미지에 127채우기 평균값이 더 나을듯
	for (int i = 0; i < oriH + 2; i++) {
		for (int k = 0; k < oriW + 2; k++) {
			tmpOriImage[i][k] = average;
		}
	}
	// 원본 --> 임시 원본
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tmpOriImage[i + 1][k + 1] = oriImage[i][k];
		}
	}
	// 회선 연산 --> 마스크로 긁어가면서 연산
	double S = 0; // 각점에 대한 마스크 연산 합계
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			S = 0.0; // 누적 초기화
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += mask[m][n] * tmpOriImage[i + m][k + n];
			tmpTarImage[i][k] = S;
		}
	}
	// Post Processing(후 처리). 마스크 합계가 0이면 127정도를 더함
	/*for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			tmpTarImage[i][k] += 127.0;
		}
	}*/
	// 임시 타켓 --> 타켓으로
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			double v = tmpTarImage[i][k];

			if (v < 0.0) {
				v = 0.0;
			}
			if (v > 255.0) {
				v = 255.0;
			}
			tarImage[i][k] = v;
		}
	}

	free_double(tmpOriImage, oriH + 2);
	free_double(tmpTarImage, tarH);
	///////////////////////////////
	displayImage();
}
void draw_circle(int oriH, int oriW, int r) {
	free_draw_circle();
	circle = (unsigned char**)malloc(sizeof(unsigned char*) * oriH);
	for (int i = 0; i < oriH; i++) {
		if (circle != NULL) {
			circle[i] = (unsigned char*)malloc(sizeof(unsigned char) * oriW);
		}

	}


	// 초기 배열 값을 0으로 설정
	for (int i = 0; i < oriH; i++) {
		for (int j = 0; j < oriW; j++) {
			circle[i][j] = 0;
		}
	}

	// 중심 좌표 계산
	int centerX = oriH / 2;
	int centerY = oriW / 2;

	// 원 그리기
	for (int x = 0; x < oriH; x++) {
		for (int y = 0; y < oriW; y++) {
			// 원의 방정식 적용
			int dx = x - centerX;
			int dy = y - centerY;
			if (dx * dx + dy * dy <= r * r) {
				circle[x][y] = 255;
			}
		}
	}
}
void free_draw_circle() {
	if (circle == NULL)
		return;
	for (int i = 0; i < tarH; i++)
		free(circle[i]);
	free(circle);
	circle = NULL;
}
void orImage() {
	/* 변수 */

	/* init */
	tarH = oriH;
	tarW = oriW;
	mallocTarImage();		// 결과 영상 메모리 할당
	draw_circle(tarH, tarW, tarH / 2);

	/* 메서드 */
	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tarImage[i][k] = oriImage[i][k] | circle[i][k];
		}
	}


	displayImage();
	// 2차원 메모리 해제
	free_draw_circle();
	freeTarImage();
}
void andImage() {
	/* 변수 */

	/* init */
	tarH = oriH;
	tarW = oriW;
	mallocTarImage();		// 결과 영상 메모리 할당
	draw_circle(tarH, tarW, tarH / 2);

	/* 메서드 */
	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tarImage[i][k] = oriImage[i][k] & circle[i][k];
		}
	}

	displayImage();
	// 2차원 메모리 해제
	free_draw_circle();
	freeTarImage();
}
void xorImage() {
	/* 변수 */
	int ans;
	/* init */
	tarH = oriH;
	tarW = oriW;
	mallocTarImage();		// 결과 영상 메모리 할당
	draw_circle(tarH, tarW, tarH / 2);

	/* 메서드 */
	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tarImage[i][k] = oriImage[i][k] ^ circle[i][k];
		}
	}


	displayImage();
	// 2차원 메모리 해제
	free_draw_circle();
	freeTarImage();
}
void notImage() {
	/* 변수 */

	/* init */
	tarH = oriH;
	tarW = oriW;
	mallocTarImage();		// 결과 영상 메모리 할당

	/* 메서드 */
	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tarImage[i][k] = ~oriImage[i][k];
		}
	}

	displayImage();
	// 2차원 메모리 해제
	freeTarImage();
}
void posterizingImage() {
	/* 변수 */
	int level[8];
	int tmp;
	/* init */
	tarH = oriH;
	tarW = oriW;
	mallocTarImage();		// 결과 영상 메모리 할당

	for (int i = 0; i < 8; i++) {
		level[i] = i * 32;
	}

	/* 메서드 */
	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tmp = oriImage[i][k] / 32;
			tarImage[i][k] = level[tmp];
		}
	}

	displayImage();
	// 2차원 메모리 해제
	freeTarImage();
}
void parabolicImage() {
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	freeTarImage();
	mallocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tarImage[i][k] = 255 * pow((oriImage[i][k] / 127) - 1, 2);
		}
	}
	///////////////////////////////
	displayImage();
}
void binarizationImage() {
	int ans;
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	mallocTarImage();

	puts(" 1.128 / 2.평균값 / 3.중앙값");
	scanf("%d", &ans);

	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tarImage[i][k] = 255 - oriImage[i][k];
		}
	}

	switch (ans)
	{
	case 1: binarizationHalf(); break;
	case 2: binarizationAverage(); break;
	case 3: binarizationCenter(); break;
	default: loadImage(); break;
	}
	///////////////////////////////
	displayImage();
	// 2차원 메모리 해제
	freeTarImage();
}
void binarizationHalf() {
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			if (tarImage[i][k] < 128) {
				tarImage[i][k] = 0;
			}
			else {
				tarImage[i][k] = 255;
			}
		}
	}
}
void binarizationAverage() {
	long long average = 0;
	long long sum = 0;
	// 합
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			sum += tarImage[i][k];
		}
	}
	// 평균
	average = sum / tarH / tarW;
	return 0;

	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			if (tarImage[i][k] < average) {
				tarImage[i][k] = 0;
			}
			else {
				tarImage[i][k] = 255;
			}
		}
	}
}
int compare(const void* a, const void* b) {
	return (*(int*)a - *(int*)b);
}
void binarizationCenter() {
	int center = 0;
	unsigned char* arr;		// 임시 1차원 배열 생성
	arr = (unsigned char*)malloc(sizeof(unsigned char) * (tarH * tarW));

	// 중앙값 
	int pos = 0;
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			arr[pos] = tarImage[i][k];
		}
	}
	qsort(arr, (tarH * tarW), sizeof(char), compare);
	center = arr[(tarH * tarW) / 2];

	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			if (tarImage[i][k] < center) {
				tarImage[i][k] = 0;
			}
			else {
				tarImage[i][k] = 255;
			}
		}
	}
	free(arr);
}
void zoomIn3()
{
	freeTarImage();
	int scale;
	printf("몇 배 확대하시겠습니까? ");
	scanf("%d", &scale);
	tarH = (int)(oriH * scale);
	tarW = (int)(oriW * scale);
	mallocTarImage();

	for (int i = 0; i < tarH; i++)
	{
		for (int j = 0; j < tarW; j++)
		{
			tarImage[i][j] = oriImage[i / scale][j / scale];
		}
	}
	int e, f, n, x, y;;

	for (int i = 0; i < tarH - scale; i += scale)
	{
		for (int j = 0; j < tarW - scale; j += scale)
		{
			for (int k = 0; k < scale; k++)
			{
				for (int l = 0; l < scale; l++)
				{
					y = i + k;
					x = j + l;
					if ((!(y == i && x == j)) && (!(y == i && x == j + scale)) && (!(y == i + scale && x == j)) && (!(y == i + scale && x == j + scale)))
					{
						e = scale * tarImage[i][j] + l * (tarImage[i][j + scale] - tarImage[i][j]);
						f = scale * tarImage[i + scale][j] + l * (tarImage[i + scale][j + scale] - tarImage[i + scale][j]);
						n = (scale - k) * e + f * k;

						if (n < 0) n = 0;
						if (n > 255) n = 255;

						tarImage[y][x] = n;
					}
				}
			}
		}
	}

	// 선형 보간법
	/*for (int i = 0; i < tarH - 1; i++)
	{
		for (int j = 0; j < tarW - 1; j++)
		{
			int x = i % scale;
			int y = j % scale;
			int x1 = i / scale;
			int y1 = j / scale;
			int x2 = x1 + 1 < oriH ? x1 + 1 : x1;
			int y2 = y1 + 1 < oriW ? y1 + 1 : y1;

			float value = (oriImage[x1][y1] * (scale - x) * (scale - y) +
				oriImage[x2][y1] * x * (scale - y) +
				oriImage[x1][y2] * (scale - x) * y +
				oriImage[x2][y2] * x * y) / (scale * scale);

			tarImage[i][j] = (int)value;
		}
	}*/
	displayImage();
}
void gammaImage() {
	/* 변수 */
	double ans = 0.0;
	double tmp = 0.0;
	/* init */
	tarH = oriH;
	tarW = oriW;
	mallocTarImage();		// 결과 영상 메모리 할당
	/* 메서드 */
	puts(" 감마 비율 input ");
	scanf("%lf", &ans);

	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tmp = pow((double)oriImage[i][k], 1.0 / ans);
			if (tmp < 0) {
				tarImage[i][k] = 0;
			}
			else if (tmp > 255) {
				tarImage[i][k] = 255;
			}
			else {
				tarImage[i][k] = (unsigned char)tmp;
			}
		}
	}

	displayImage();
	// 2차원 메모리 해제
	freeTarImage();
}