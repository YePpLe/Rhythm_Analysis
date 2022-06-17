// 2303 남태웅 2022 데이터 분석 프로그램  
// 주제: 사용자의 박자 감각 분석 프로그램  
// 참고: https://coding-factory.tistory.com/category/Language/C%20%20%2C%20%20%20C%2B%2B%20%20%2C%20%20C%23
// 프로그램 설명: 
// 해결 아이디어 : while로 clock체크, 하자마자 playing true 스레드 실행 사운드 출력, 1ms Sleep, playing false

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // bool 자료형  
#include <windows.h> // 윈도우 콘솔창 구성 
#include <process.h> // 다중 스레드  
#include <time.h> // 시간 측정 및 제어를 위한 헤더파일  
#include <conio.h> // _getch() 사용  
#include <mmsystem.h> // 소리 출력 

void menu(); // 메뉴 구성 화면 
void analysis(); // 이 프로그램의 주 기능인 박자 분석 및 기록 저장 
void load(); // 과거 기록을 불러온 후 비교 
void playingMetronomeSound(void* n); // 스레드로 사운드 재생 

bool playing = false;
int timesample;

int main()
{
	printf("%lld", timeGetTime());
	analysis();
	system("pause");
}

void analysis()
{
	int i = 0, t = 300;
	long long start_time = clock(), input_time[10] = {0, };
	unsigned char input = 0;
	playing = true;
	_beginthread(playingMetronomeSound, 0, (void*)t);
	while(i < 10)
	{
		if(_kbhit()) {
			input = _getch();
			input_time[i] = clock();
			i++;
		}
	}
	playing = false;
	printf("시작 시간: %d\n", start_time);
	for(i = 0; i < 10; i++)
		printf("%d ", input_time[i]);
}

void playingMetronomeSound(void* n)
{
	Sleep(1000);
	while(playing) {
		printf("%lld\n", timeGetTime());
		PlaySound("Metronome.wav", NULL, SND_ASYNC);
		Sleep((int)n-10);
	}
}
