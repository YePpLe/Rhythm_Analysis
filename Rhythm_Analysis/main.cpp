// 2303 남태웅 2022 데이터 분석 프로그램  
// 주제: 사용자의 박자 감각 분석 프로그램  

#include <stdio.h>
#include <math.h> // sqrt 등의 수학 관련 함수 
#include <windows.h> // 윈도우 콘솔창 구성 
#include <time.h> // 시간 측정 및 제어를 위한 헤더파일 
#include <conio.h> // _getch() 사용 
#include <mmsystem.h> // 소리 출력  

typedef struct result {
	int BPM, devitation[10]; // BPM, 원래 박자와의 차이 
	double avg, sd; // avg: 평균, sd: 표준편차 
} Result;

void init_menu(); // 메뉴 구성 화면 
void sync_adjustment(); // 싱크 조절 함수 
void analysis(); // 이 프로그램의 주 기능인 박자 분석 
void save_load(char mod, Result res); // 현재 기록 저장 및 과거 기록을 불러옴 
bool load(char n, Result data[]); // 과거 기록 불러오고 띄움 
bool compare(char n, Result data[], unsigned char compare1); // 기록끼리 비교 
void arrowmove1(int a, int b, int c); // 방향키 상하 움직임 함수 
void arrowmove2(int a, int b, int c); // 방향키 좌우 움직임 함수 
void gotoxy(int x, int y);

CONSOLE_CURSOR_INFO cursorInfo = {1, FALSE}; // 커서 설정 

bool running = true; // 실행 제어 
unsigned char input, current_pos; // input: 키보드 입력받은것 저장, current_pos: 방향키 움직임 위치 
int sync = 0; // 싱크의 기본값 

int main()
{
	SetConsoleTitle("박자 감각 분석 프로그램"); // 프로그램 제목 지정 
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo); // 커서 숨기기 
    
    while(running)
    	init_menu();
}

void init_menu()
{
	system("mode con:cols=51 lines=16"); // 콘솔창 크기 설정 
	
	current_pos = 8;
	
	printf("===================================================\n");
	printf("                                                   \n");
	printf("              박자 감각 분석 프로그램              \n");
	printf("                                       2303 남태웅 \n");
    printf("                                                   \n");
    printf("           방향키로 움직이고 엔터로 선택           \n");
    printf("===================================================\n\n");
	printf("                   > 박자 분석                     \n"); // 8번째 줄 
	printf("                     기록 조회                     \n"); // 9
	printf("                     싱크 조절                     \n"); // 10
	printf("                       종료                        \n"); // 11
	
	// 메뉴 선택 구현 
	while(1)
	{
		// 입력을 받았을 때 
		if(_kbhit()) {
			input = _getch();
			// 방향키 움직임 구현 
			if(input == 224 || input == 0) {
				input = _getch();
				switch(input) {
					case 72: // 위 방향키 
						if(current_pos > 8) {
							arrowmove1(current_pos, current_pos-1, 19);
							current_pos--;
						}
						break;
					case 80: // 아래 방향키 
						if(current_pos < 11) {
							arrowmove1(current_pos, current_pos+1, 19);
							current_pos++;
						}
						break;
				}
			}
			// 엔터키 구현 
			else if(input == 13) {
				switch(current_pos) {
					case 8: // 박자 분석을 선택했을때 
						analysis();
						return; 
						break;
					case 9: { // 기록 조회를 선택했을때 
						Result tmp; 
						save_load(0, tmp);
						return;
						break;
					}
					case 10: // 싱크 조절을 선택했을때 
						sync_adjustment();
						return;
						break;
					case 11: // 종료를 선택했을때 
						running = false;
						return;
						break;
				}
			}
		}
	}
}

void sync_adjustment()
{
	system("cls");
	system("mode con:cols=61 lines=16");
	printf("싱크를 조절합니다.\n\n");
	printf(" > 자동   수동   뒤로가기\n");
	printf("사용자의 입력과 소리의 차이로 싱크를 조절합니다."); 
	current_pos = 1; 
	while(1)
	{
		// 입력을 받았을 때 
		if(_kbhit()) {
			input = _getch();
			// 방향키 움직임 구현 
			if(input == 224 || input == 0) {
				input = _getch();
				switch(input) {
					case 75: // 왼쪽 방향키 
						if(current_pos > 1) {
							arrowmove2(current_pos, current_pos-7, 2);
							current_pos-=7;
							if(current_pos == 1) {
								gotoxy(0, 3);
								printf("사용자의 입력과 소리의 차이로 싱크를 조절합니다."); 
							} 
							else if(current_pos == 8) {
								gotoxy(0, 3);
								printf("사용자가 직접 싱크를 조절합니다.                                   "); 
							}
						}
						break;
					case 77: // 오른쪽 방향키 
						if(current_pos < 15) {
							arrowmove2(current_pos, current_pos+7, 2);
							current_pos+=7;
							if(current_pos == 8) {
								gotoxy(0, 3);
								printf("사용자가 직접 싱크를 조절합니다.                                   "); 
							} 
							else if(current_pos == 15) {
								gotoxy(0, 3);
								printf("메인화면으로 돌아갑니다.                                           "); 
							}
						}
						break;
				}
			}
			// 엔터키 구현 
			else if(input == 13) {
				switch(current_pos) {
					case 1: {// 자동이라면 
						// 싱크 조절 안내 
						system("cls");
						PlaySound("Metronome.wav", NULL, SND_ASYNC); // 이걸 안해주면 정확한 박자로 출력 x 
						printf("3번째 박자의 메트로놈 소리에 맞추어 아무키나 입력하세요.\n");
						printf("아무키나 입력하면 시작합니다.\n");
						while(!_kbhit());
						input = _getch();
						if(input == 224 || input == 0) _getch(); // 예외처리 
						
						printf("  [1]");
						for(int i = 2; i <= 3; i++)
							printf("    [%d]", i);
						printf("\n");
						
						sync = 0;
						int start_time, next_time, input_time; // start_time: 최초 시작 시간, next_time: 다음 재생 시간, input_time: 입력 받았을 때의 시간 
						unsigned char cnt = 0; // 소리 재생 횟수 체크 
						while(clock()%1000); // 첫 출력 시간을 1초 단위로 만들어 주기 위한 과정 
						next_time = clock();
						start_time = next_time + 1200;
						while(cnt < 10)
						{
							// 일정한 간격으로 메트로놈 출력을 위한 구문 
							if(clock() >= next_time) {
								if(cnt == 9) break; 
								PlaySound("Metronome.wav", NULL, SND_ASYNC);
								next_time += cnt % 3 == 2 ? 1200 : 600;
								cnt++;
							} 
							// 사용자의 입력을 받는 구문 
							if(_kbhit()) {
								input_time = clock();
								input = _getch();
								// 특수키(방향키 혹은 펑션)를 눌렀다면 남은 것을 지워줌 
								if(input == 224 || input == 0)
									_getch();
								printf("%4dms ", input_time - start_time);
								sync += input_time - start_time;
								start_time += 2400; 
							}
						}
						sync /= 3;
						
						// 싱크 저장 
						FILE* fp = fopen("users_record/sync.txt", "w");
						fprintf(fp, "%d", sync);
						fclose(fp);
						
						printf("\n싱크 조정값: %dms\n", sync);
						printf("싱크가 조절되었습니다. 2초 뒤 메인화면으로 넘어갑니다.");
						
						Sleep(2000);
						return;
						break;
					}
					case 8: { // 수동이라면 
						// 커서 보임 
						cursorInfo.bVisible = TRUE;
						SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
						
						unsigned char tmp = 0; // tmp: 예외처리를 위한 변수 
						do
						{
							system("cls");
							printf("보정할 싱크값을 입력해주세요: \n");
							printf("내 입력이 소리출력보다 느리다면 (+)값\n더 빠르다면 (-)값으로 입력해주세요.\n");
							gotoxy(30, 0);
							tmp = scanf("%d", &sync); // 정수가 아닌 값을 입력하면 0을 반환함 
							while(getchar() != '\n'); // 입력버퍼 지우기 
						} while(!tmp);
						
						// 싱크 저장 
						FILE* fp = fopen("users_record/sync.txt", "w");
						fprintf(fp, "%d", sync);
						fclose(fp);
						
						// 커서 지움 
						cursorInfo.bVisible = FALSE;
						SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
						
						gotoxy(0, 3);
						printf("저장되었습니다.");
						Sleep(500);
						return;
						break;
					}
					case 15: // 뒤로가기라면 
						return;
						break; 
				}
			}
		}
	}
}

void analysis()
{
	// 커서를 보이고 창 크기 조정 
	cursorInfo.bVisible = TRUE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	system("mode con:cols=95 lines=16");
	
	// 싱크값 불러오기 
	FILE* fp = fopen("users_record/sync.txt", "r");
	if(fp == NULL) {
		fp = fopen("users_record/sync.txt", "w"); 
		fprintf(fp, "%d", 0);
		fclose(fp);
		fp = fopen("users_record/sync.txt", "r");
	}
	fscanf(fp, "%d", &sync);
	fclose(fp);
	
	PlaySound("Metronome.wav", NULL, SND_ASYNC); // 이걸 안해주면 정확한 박자로 출력 x 
	int cnt = 0, BPM, t, tmp = 0; // cnt: 입력 횟수 체크, BPM: BPM 저장, t: 박자 간 간격, tmp: 예외처리를 위한 변수 
	do
	{
		system("cls");
		printf("원하시는 BPM을 입력해주세요 : "); 
		tmp = scanf("%d", &t); // 정수가 아닌 값을 입력하면 0을 반환함 
		while(getchar() != '\n'); // 입력버퍼 지우기 
	} while(!tmp || t<0);
	BPM = t;
	t = 60000/t;
	
	// 커서 지움 
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	
	system("cls");
	printf("원하시는 BPM을 입력해주세요 : %d\n", BPM); 
	// 사용 방법 안내와 컴퓨터에 따라서 BPM이 너무 빠르면 소리가 출력되지 않을 수 있으므로 안내 
	printf("\n메트로놈 소리를 듣고 박자에 맞추어 아무키나 10번 입력하세요\n");
	printf("만약 소리가 출력되지 않거나 다시하고 싶다면 ESC를 눌러주세요\n"); 
	// 즉각적으로 타이밍을 표시 
	printf("\n타이밍 : 메트로놈 출력 소리보다 (+)는 느림, (-)는 빠름\n  [1]");
	for(int i = 2; i <= 9; i++)
		printf("    [%d]", i);
	printf("   [10]\n");
	
	Result res;
	res.BPM = BPM;
	int start_time, next_time, input_time[10] = {0, }; // start_time: 최초 시작 시간, next_time: 다음 재생 시간, input_time: 입력 받았을 때의 시간 
	int sumx = 0, sumx2 = 0; // 평균과 표준편차 계산을 위한 변수 
	while(clock()%1000); // 첫 출력 시간을 1초 단위로 만들어 주기 위한 과정 
	next_time = clock();
	while(cnt < 10)
	{
		// 일정한 간격으로 메트로놈 출력을 위한 구문 
		if(clock() >= next_time) {
			PlaySound("Metronome.wav", NULL, SND_ASYNC);
			next_time += t;
		} 
		// 사용자의 입력을 받는 구문 
		if(_kbhit()) {
			input_time[cnt] = clock() - sync;
			input = _getch();
			// 특수키(방향키 혹은 펑션)를 눌렀다면 남은 것을 지워줌 
			if(input == 224 || input == 0)
				_getch();
			// esc를 눌렀다면 재시작 
			else if(input == 27) {
				analysis();
				return;
			}
			// 최초 시작 시간 체크(이후로 간격이 이 기준) 
			if(!cnt) {
				int delay = 0;
				while(input_time[cnt] - (next_time - t - delay) < 0) delay += t;
				
				if(input_time[cnt] - (next_time - t - delay) < (next_time - delay) - input_time[cnt]) 
					start_time = next_time - t - delay;
				else 
					start_time = next_time - delay;
			}
			
			// 0ms와의 간격 저장 
			res.devitation[cnt] = input_time[cnt] - start_time;
			printf("%4dms ", res.devitation[cnt]); 
			sumx += res.devitation[cnt];
			sumx2 += res.devitation[cnt]*res.devitation[cnt];
			start_time += t; 
			cnt++;
		}
	}
	// 원래 소리 출력보다 빨리 눌러서 안된 소리 출력 
	if(res.devitation[9] < 0) {
		while(clock() <= next_time);
		PlaySound("Metronome.wav", NULL, SND_ASYNC);
	} 
	
	// 평균 및 표준편차 출력 
	res.avg = (double)sumx/10.0; res.sd = sqrt((double)sumx2/10.0 - res.avg*res.avg); // res.avg: 평균, res.sd: 표준편차(sqrt(E(X^2) - (E(X))^2)) 
	printf("\n평균 : %.1lfms, 표준 편차 : %.2lf\n", res.avg, res.sd);
	printf("\n상업 리듬게임의 평균적인 정확 판정(±40ms)보다 표준편차가 %s", res.sd <= 40 ? "작습니다." : "큽니다."); 
	save_load(2, res);
	
	current_pos = 25; 
	printf("\n기록을 저장하시겠습니까? > 예     아니오\n"); // 예: 25, 아니오: 32 
	printf("최근 10개의 데이터만 저장되므로 기록이 사라질 수도 있습니다.");
	while(1)
	{
		if(_kbhit()) {
			input = _getch();
			// 방향키 움직임 구현 
			if(input == 224 || input == 0) {
				input = _getch();
				switch(input) {
					case 75: // 왼쪽 방향키 
						if(current_pos == 32) {
							arrowmove2(current_pos, 25, 14);
							current_pos = 25;
						}
						break;
					case 77: // 오른쪽 방향키 
						if(current_pos == 25) {
							arrowmove2(current_pos, 32, 14);
							current_pos = 32;
						}
						break;
				}
			}
			// 엔터키 구현 
			else if(input == 13) {
				switch(current_pos) {
					case 25: // 예를 선택했을때 
						save_load(1, res);
						gotoxy(25, 14);
						printf("저장되었습니다.");
						Sleep(500);
						return; 
						break;
					case 32: // 아니오를 선택했을때 
						return;
						break;
				}
			}
		}
	}
}

void save_load(char mod, Result res)
{
	FILE* fp = fopen("users_record/users_record.csv", "r");
	// 처음 실행하여 파일이 없다면 새로 생성 
	if(fp == NULL) {
		fp = fopen("users_record/users_record.csv", "w");
		fprintf(fp, "%d\n", 0);
		fclose(fp);
		fp = fopen("users_record/users_record.csv", "r");
	}
	char n; // n: 데이터 개수 
	fscanf(fp, "%d", &n);
	Result data[10];
	// 데이터 정보들을 가져옴 
	for(int i = 0; i < n; i++)
	{
		fscanf(fp, "%d", &data[i].BPM);
		for(int j = 0; j < 10; j++)
			fscanf(fp, "%d", &data[i].devitation[j]);
		fscanf(fp, "%lf %lf", &data[i].avg, &data[i].sd);
	}
	fclose(fp); 
	
	// 박자 분석을 한 후 지난 기록과 비교를 하는거면 
	if(mod == 2) {
		// 데이터가 없다면 리턴 
		if(n == 0) {
			printf("\n최초 기록입니다.\n\n");
			return;
		}
		double sv_avg, sv_sd; // sv_avg: 저장된 기록의 평균의 평균, sv_sd: 저장된 기록의 평균의 표준편차 
		// 저장된 기록의 평균과 표준편차의 평균을 구함 
		for(int i = 0; i < n; i++)
		{
			sv_avg += data[i].avg; sv_sd += data[i].sd;
		}
		sv_avg /= n; sv_sd /= n;
		// 기록 비교 
		double avg_diff = fabs(res.avg) - fabs(sv_avg), sd_diff = res.sd - sv_sd;
		printf("\n저장된 기록 대비 %.1lfms정도 0ms와의 차이가 %s ", fabs(avg_diff), avg_diff > 0 ? "벌어졌고" : "줄어들었고"); 
		printf("표준편차는 %.2lf정도 %s", fabs(sd_diff), sd_diff > 0 ? "증가하였습니다.\n" : "감소하였습니다.\n");
		if(avg_diff <= 0 && sd_diff <= 0) 
			printf("평균과 표준편차 모두 저장된 기록보다 좋습니다.\n");
		else if(avg_diff > 0 && sd_diff <= 0) 
			printf("평균은 저장된 기록보다 0ms와의 차이가 크지만 표준편차가 더 작으므로 박자가 더 일정합니다.\n"); 
		else if(avg_diff <= 0 && sd_diff > 0) 
			printf("평균은 저장된 기록보다 0ms와의 차이가 작지만 표준편차가 더 크므로 박자가 불안정합니다.\n");
		else 
			printf("평균과 표준편차 모두 저장된 기록보다 저조합니다.\n");
	}
	// 저장을 하는거면 
	else if(mod == 1) {
		fp = fopen("users_record/users_record.csv", "w");
		// 데이터가 10개라면 
		if(n == 10) {
			fprintf(fp, "%d\n", n); // 제일 상단에 데이터 개수 출력 
			// 최근 9개의 데이터만 기록 
			for(int i = 1; i < 10; i++)
			{
				fprintf(fp, "%d ", data[i].BPM);
				for(int j = 0; j < 10; j++)
					fprintf(fp, "%d ", data[i].devitation[j]);
				fprintf(fp, "%.1lf %.2lf\n", data[i].avg, data[i].sd);
			}
		}
		// 그보다 적다면 
		else {
			fprintf(fp, "%d\n", n+1); // 제일 상단에 데이터 개수 출력 
			for(int i = 0; i < n; i++) // 최근의 모든 데이터 기록 
			{
				fprintf(fp, "%d ", data[i].BPM);
				for(int j = 0; j < 10; j++)
					fprintf(fp, "%d ", data[i].devitation[j]);
				fprintf(fp, "%.1lf %.2lf\n", data[i].avg, data[i].sd);
			}
		}
		fclose(fp);
		// 데이터 저장 
		fp = fopen("users_record/users_record.csv", "a");
		fprintf(fp, "%d ", res.BPM);
		for(int i = 0; i < 10; i++)
			fprintf(fp, "%d ", res.devitation[i]);
		fprintf(fp, "%.1lf %.2lf\n", res.avg, res.sd);
		fclose(fp);
	}
	// 과거 기록을 불러 오는거면 
	else {
		bool IsLoad = true;
		while(IsLoad)
			IsLoad = load(n, data);
	}
		
}

bool load(char n, Result data[])
{
	system("cls");
	system("mode con:cols=51 lines=16");
	// 기록이 없다면 
	if(n == 0) {
		printf("기록이 없습니다!");
		Sleep(1000);
		return false;
	} 
	// [1]부터 [n]까지의 기록 띄우기 
	printf(" > [1] %dBPM\n", data[0].BPM);
	for(int i = 1; i < n; i++)
		printf("   [%d] %dBPM\n", i+1, data[i].BPM);
	printf("   종료\n");
	
	unsigned char compare1; // compare1: 비교할 데이터 인덱스
	current_pos = 0; 
	while(1)
	{
		// 입력을 받았을 때 
		if(_kbhit()) {
			input = _getch();
			// 방향키 움직임 구현 
			if(input == 224 || input == 0) {
				input = _getch();
				switch(input) {
					case 72: // 위 방향키 
						if(current_pos > 0) {
							arrowmove1(current_pos, current_pos-1, 1);
							current_pos--;
						}
						else {
							arrowmove1(current_pos, n, 1);
							current_pos = n;
						}
						break;
					case 80: // 아래 방향키 
						if(current_pos < n) {
							arrowmove1(current_pos, current_pos+1, 1);
							current_pos++;
						}
						else {
							arrowmove1(current_pos, 0, 1);
							current_pos = 0;
						}
						break;
				}
			}
			// 엔터키 구현 
			else if(input == 13) {
				if(current_pos == n) return false;
				compare1 = current_pos;
				system("cls");
				system("mode con:cols=95 lines=16");
				printf("BPM : %d\n", data[current_pos].BPM);
				printf("타이밍 : 메트로놈 출력 소리보다 (+)는 느림, (-)는 빠름\n  [1]");
				for(int i = 2; i <= 10; i++)
					printf("    [%d]", i);
				printf("\n");
				for(int i = 0; i < 10; i++)
					printf("%4dms ", data[current_pos].devitation[i]);
				printf("\n");
				printf("\n평균 : %.1lfms, 표준 편차 : %.2lf\n", data[current_pos].avg, data[current_pos].sd);
				printf("\n > 비교하기   뒤로가기");
				current_pos = 1;
				while(1)
				{
					if(_kbhit()) {
						input = _getch();
						// 방향키 입력 
						if(input == 224 || input == 0) {
							input = _getch();
							switch(input) {
								case 75: // 왼쪽 방향키 
									if(current_pos == 12) {
										arrowmove2(current_pos, 1, 7);
										current_pos = 1;
									}
									break;
								case 77: // 오른쪽 방향키 
									if(current_pos == 1) {
										arrowmove2(current_pos, 12, 7);
										current_pos = 12;
									}
									break;
							}
						}
						// 엔터키 입력 
						else if(input == 13) {
							switch(current_pos) {
								case 1: // 비교하기를 누르면 
									return compare(n, data, compare1);
									break;
								case 12: // 뒤로가기를 누르면 
									return true;
									break;
							}
						}
					}
				}
			}
		}
	}
}

bool compare(char n, Result data[], unsigned char compare1) 
{
	system("cls");
	system("mode con:cols=51 lines=16");
	printf("비교할 기록을 골라주세요.\n");
	// [1] 부터 [n]까지의 기록 띄우기 
	printf(" > [1] %dBPM\n", data[0].BPM);
	for(int i = 1; i < n; i++)
		printf("   [%d] %dBPM\n", i+1, data[i].BPM);
	printf("   종료\n");
	
	gotoxy(2, 1);
	current_pos = 1;
	while(1)
	{
		// 입력을 받았을 때 
		if(_kbhit()) {
			input = _getch();
			// 방향키 움직임 구현 
			if(input == 224 || input == 0) {
				input = _getch();
				switch(input) {
					case 72: // 위 방향키 
						if(current_pos > 1) {
							arrowmove1(current_pos, current_pos-1, 1);
							current_pos--;
						}
						else {
							arrowmove1(current_pos, n+1, 1);
							current_pos = n+1;
						}
						break;
					case 80: // 아래 방향키 
						if(current_pos <= n) {
							arrowmove1(current_pos, current_pos+1, 1);
							current_pos++;
						}
						else {
							arrowmove1(current_pos, 1, 1);
							current_pos = 1;
						}
						break;
				}
			}
			// 엔터키 구현 
			else if(input == 13) {
				if(current_pos == n+1) return false;
				current_pos--;
				if(current_pos == compare1) {
					printf(" 동일한 기록입니다.");
					current_pos++;
					continue;
				}
				// 선택한 두 기록을 화면에 표시 
				system("cls");
				system("mode con:cols=150 lines=16");
				printf("BPM : %d", data[compare1].BPM);
				gotoxy(70, 0);
				printf("│ BPM : %d\n", data[current_pos].BPM);
				printf("타이밍 : 메트로놈 출력 소리보다 (+)는 느림, (-)는 빠름");
				gotoxy(70, 1);
				printf("│\n  [1]");
				for(int i = 2; i <= 10; i++)
					printf("    [%d]", i);
				printf(" │  [1]");
				for(int i = 2; i <= 10; i++)
					printf("    [%d]", i);
				printf("\n");
				for(int i = 0; i < 10; i++)
					printf("%4dms ", data[compare1].devitation[i]);
				printf("│");
				for(int i = 0; i < 10; i++)
					printf("%4dms ", data[current_pos].devitation[i]);
				gotoxy(70, 4);
				printf("│");
				printf("\n평균 : %.1lfms, 표준 편차 : %.2lf", data[compare1].avg, data[compare1].sd);
				gotoxy(70, 5);
				printf("│ ");
				printf("평균 : %.1lfms, 표준 편차 : %.2lf\n", data[current_pos].avg, data[current_pos].sd);
				
				// 기록 비교 
				double avg_diff = fabs(data[compare1].avg) - fabs(data[current_pos].avg), sd_diff = data[compare1].sd - data[current_pos].sd;
				printf("\n비교하는 기록 대비 %.1lfms정도 0ms와의 차이가 %s ", fabs(avg_diff), avg_diff > 0 ? "벌어졌고" : "줄어들었고"); 
				printf("표준편차는 %.2lf정도 %s", fabs(sd_diff), sd_diff > 0 ? "증가하였습니다.\n" : "감소하였습니다.\n");
				if(avg_diff < 0 && sd_diff < 0) 
					printf("평균과 표준편차 모두 비교하는 기록보다 좋습니다.\n");
				else if(avg_diff > 0 && sd_diff < 0) 
					printf("평균은 비교하는 기록보다 0ms와의 차이가 크지만 표준편차가 더 작으므로 박자가 더 일정합니다.\n"); 
				else if(avg_diff < 0 && sd_diff > 0) 
					printf("평균은 비교하는 기록보다 0ms와의 차이가 작지만 표준편차가 더 크므로 박자가 불안정합니다.\n");
				else 
					printf("평균과 표준편차 모두 비교하는 기록보다 저조합니다.\n");
				printf("\n아무키나 누르면 기록 조회 화면으로 돌아갑니다.");
				while(!_kbhit());
				_getch();
				return true; 
			}
		}
	}
}

void arrowmove1(int a, int b, int c)
{
	gotoxy(c, a);
	printf(" ");
	gotoxy(c, b);
	printf(">");
}

void arrowmove2(int a, int b, int c)
{
	gotoxy(a, c);
	printf(" ");
	gotoxy(b, c);
	printf(">");
}

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
