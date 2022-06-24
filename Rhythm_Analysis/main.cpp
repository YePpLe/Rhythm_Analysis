// 2303 ���¿� 2022 ������ �м� ���α׷�  
// ����: ������� ���� ���� �м� ���α׷�  

#include <stdio.h>
#include <math.h> // sqrt ���� ���� ���� �Լ� 
#include <windows.h> // ������ �ܼ�â ���� 
#include <time.h> // �ð� ���� �� ��� ���� ������� 
#include <conio.h> // _getch() ��� 
#include <mmsystem.h> // �Ҹ� ���  

typedef struct result {
	int BPM, devitation[10]; // BPM, ���� ���ڿ��� ���� 
	double avg, sd; // avg: ���, sd: ǥ������ 
} Result;

void init_menu(); // �޴� ���� ȭ�� 
void sync_adjustment(); // ��ũ ���� �Լ� 
void analysis(); // �� ���α׷��� �� ����� ���� �м� 
void save_load(char mod, Result res); // ���� ��� ���� �� ���� ����� �ҷ��� 
bool load(char n, Result data[]); // ���� ��� �ҷ����� ��� 
bool compare(char n, Result data[], unsigned char compare1); // ��ϳ��� �� 
void arrowmove1(int a, int b, int c); // ����Ű ���� ������ �Լ� 
void arrowmove2(int a, int b, int c); // ����Ű �¿� ������ �Լ� 
void gotoxy(int x, int y);

CONSOLE_CURSOR_INFO cursorInfo = {1, FALSE}; // Ŀ�� ���� 

bool running = true; // ���� ���� 
unsigned char input, current_pos; // input: Ű���� �Է¹����� ����, current_pos: ����Ű ������ ��ġ 
int sync = 0; // ��ũ�� �⺻�� 

int main()
{
	SetConsoleTitle("���� ���� �м� ���α׷�"); // ���α׷� ���� ���� 
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo); // Ŀ�� ����� 
    
    while(running)
    	init_menu();
}

void init_menu()
{
	system("mode con:cols=51 lines=16"); // �ܼ�â ũ�� ���� 
	
	current_pos = 8;
	
	printf("===================================================\n");
	printf("                                                   \n");
	printf("              ���� ���� �м� ���α׷�              \n");
	printf("                                       2303 ���¿� \n");
    printf("                                                   \n");
    printf("           ����Ű�� �����̰� ���ͷ� ����           \n");
    printf("===================================================\n\n");
	printf("                   > ���� �м�                     \n"); // 8��° �� 
	printf("                     ��� ��ȸ                     \n"); // 9
	printf("                     ��ũ ����                     \n"); // 10
	printf("                       ����                        \n"); // 11
	
	// �޴� ���� ���� 
	while(1)
	{
		// �Է��� �޾��� �� 
		if(_kbhit()) {
			input = _getch();
			// ����Ű ������ ���� 
			if(input == 224 || input == 0) {
				input = _getch();
				switch(input) {
					case 72: // �� ����Ű 
						if(current_pos > 8) {
							arrowmove1(current_pos, current_pos-1, 19);
							current_pos--;
						}
						break;
					case 80: // �Ʒ� ����Ű 
						if(current_pos < 11) {
							arrowmove1(current_pos, current_pos+1, 19);
							current_pos++;
						}
						break;
				}
			}
			// ����Ű ���� 
			else if(input == 13) {
				switch(current_pos) {
					case 8: // ���� �м��� ���������� 
						analysis();
						return; 
						break;
					case 9: { // ��� ��ȸ�� ���������� 
						Result tmp; 
						save_load(0, tmp);
						return;
						break;
					}
					case 10: // ��ũ ������ ���������� 
						sync_adjustment();
						return;
						break;
					case 11: // ���Ḧ ���������� 
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
	printf("��ũ�� �����մϴ�.\n\n");
	printf(" > �ڵ�   ����   �ڷΰ���\n");
	printf("������� �Է°� �Ҹ��� ���̷� ��ũ�� �����մϴ�."); 
	current_pos = 1; 
	while(1)
	{
		// �Է��� �޾��� �� 
		if(_kbhit()) {
			input = _getch();
			// ����Ű ������ ���� 
			if(input == 224 || input == 0) {
				input = _getch();
				switch(input) {
					case 75: // ���� ����Ű 
						if(current_pos > 1) {
							arrowmove2(current_pos, current_pos-7, 2);
							current_pos-=7;
							if(current_pos == 1) {
								gotoxy(0, 3);
								printf("������� �Է°� �Ҹ��� ���̷� ��ũ�� �����մϴ�."); 
							} 
							else if(current_pos == 8) {
								gotoxy(0, 3);
								printf("����ڰ� ���� ��ũ�� �����մϴ�.                                   "); 
							}
						}
						break;
					case 77: // ������ ����Ű 
						if(current_pos < 15) {
							arrowmove2(current_pos, current_pos+7, 2);
							current_pos+=7;
							if(current_pos == 8) {
								gotoxy(0, 3);
								printf("����ڰ� ���� ��ũ�� �����մϴ�.                                   "); 
							} 
							else if(current_pos == 15) {
								gotoxy(0, 3);
								printf("����ȭ������ ���ư��ϴ�.                                           "); 
							}
						}
						break;
				}
			}
			// ����Ű ���� 
			else if(input == 13) {
				switch(current_pos) {
					case 1: {// �ڵ��̶�� 
						// ��ũ ���� �ȳ� 
						system("cls");
						PlaySound("Metronome.wav", NULL, SND_ASYNC); // �̰� �����ָ� ��Ȯ�� ���ڷ� ��� x 
						printf("3��° ������ ��Ʈ�γ� �Ҹ��� ���߾� �ƹ�Ű�� �Է��ϼ���.\n");
						printf("�ƹ�Ű�� �Է��ϸ� �����մϴ�.\n");
						while(!_kbhit());
						input = _getch();
						if(input == 224 || input == 0) _getch(); // ����ó�� 
						
						printf("  [1]");
						for(int i = 2; i <= 3; i++)
							printf("    [%d]", i);
						printf("\n");
						
						sync = 0;
						int start_time, next_time, input_time; // start_time: ���� ���� �ð�, next_time: ���� ��� �ð�, input_time: �Է� �޾��� ���� �ð� 
						unsigned char cnt = 0; // �Ҹ� ��� Ƚ�� üũ 
						while(clock()%1000); // ù ��� �ð��� 1�� ������ ����� �ֱ� ���� ���� 
						next_time = clock();
						start_time = next_time + 1200;
						while(cnt < 10)
						{
							// ������ �������� ��Ʈ�γ� ����� ���� ���� 
							if(clock() >= next_time) {
								if(cnt == 9) break; 
								PlaySound("Metronome.wav", NULL, SND_ASYNC);
								next_time += cnt % 3 == 2 ? 1200 : 600;
								cnt++;
							} 
							// ������� �Է��� �޴� ���� 
							if(_kbhit()) {
								input_time = clock();
								input = _getch();
								// Ư��Ű(����Ű Ȥ�� ���)�� �����ٸ� ���� ���� ������ 
								if(input == 224 || input == 0)
									_getch();
								printf("%4dms ", input_time - start_time);
								sync += input_time - start_time;
								start_time += 2400; 
							}
						}
						sync /= 3;
						
						// ��ũ ���� 
						FILE* fp = fopen("users_record/sync.txt", "w");
						fprintf(fp, "%d", sync);
						fclose(fp);
						
						printf("\n��ũ ������: %dms\n", sync);
						printf("��ũ�� �����Ǿ����ϴ�. 2�� �� ����ȭ������ �Ѿ�ϴ�.");
						
						Sleep(2000);
						return;
						break;
					}
					case 8: { // �����̶�� 
						// Ŀ�� ���� 
						cursorInfo.bVisible = TRUE;
						SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
						
						unsigned char tmp = 0; // tmp: ����ó���� ���� ���� 
						do
						{
							system("cls");
							printf("������ ��ũ���� �Է����ּ���: \n");
							printf("�� �Է��� �Ҹ���º��� �����ٸ� (+)��\n�� �����ٸ� (-)������ �Է����ּ���.\n");
							gotoxy(30, 0);
							tmp = scanf("%d", &sync); // ������ �ƴ� ���� �Է��ϸ� 0�� ��ȯ�� 
							while(getchar() != '\n'); // �Է¹��� ����� 
						} while(!tmp);
						
						// ��ũ ���� 
						FILE* fp = fopen("users_record/sync.txt", "w");
						fprintf(fp, "%d", sync);
						fclose(fp);
						
						// Ŀ�� ���� 
						cursorInfo.bVisible = FALSE;
						SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
						
						gotoxy(0, 3);
						printf("����Ǿ����ϴ�.");
						Sleep(500);
						return;
						break;
					}
					case 15: // �ڷΰ����� 
						return;
						break; 
				}
			}
		}
	}
}

void analysis()
{
	// Ŀ���� ���̰� â ũ�� ���� 
	cursorInfo.bVisible = TRUE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	system("mode con:cols=95 lines=16");
	
	// ��ũ�� �ҷ����� 
	FILE* fp = fopen("users_record/sync.txt", "r");
	if(fp == NULL) {
		fp = fopen("users_record/sync.txt", "w"); 
		fprintf(fp, "%d", 0);
		fclose(fp);
		fp = fopen("users_record/sync.txt", "r");
	}
	fscanf(fp, "%d", &sync);
	fclose(fp);
	
	PlaySound("Metronome.wav", NULL, SND_ASYNC); // �̰� �����ָ� ��Ȯ�� ���ڷ� ��� x 
	int cnt = 0, BPM, t, tmp = 0; // cnt: �Է� Ƚ�� üũ, BPM: BPM ����, t: ���� �� ����, tmp: ����ó���� ���� ���� 
	do
	{
		system("cls");
		printf("���Ͻô� BPM�� �Է����ּ��� : "); 
		tmp = scanf("%d", &t); // ������ �ƴ� ���� �Է��ϸ� 0�� ��ȯ�� 
		while(getchar() != '\n'); // �Է¹��� ����� 
	} while(!tmp || t<0);
	BPM = t;
	t = 60000/t;
	
	// Ŀ�� ���� 
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	
	system("cls");
	printf("���Ͻô� BPM�� �Է����ּ��� : %d\n", BPM); 
	// ��� ��� �ȳ��� ��ǻ�Ϳ� ���� BPM�� �ʹ� ������ �Ҹ��� ��µ��� ���� �� �����Ƿ� �ȳ� 
	printf("\n��Ʈ�γ� �Ҹ��� ��� ���ڿ� ���߾� �ƹ�Ű�� 10�� �Է��ϼ���\n");
	printf("���� �Ҹ��� ��µ��� �ʰų� �ٽ��ϰ� �ʹٸ� ESC�� �����ּ���\n"); 
	// �ﰢ������ Ÿ�̹��� ǥ�� 
	printf("\nŸ�̹� : ��Ʈ�γ� ��� �Ҹ����� (+)�� ����, (-)�� ����\n  [1]");
	for(int i = 2; i <= 9; i++)
		printf("    [%d]", i);
	printf("   [10]\n");
	
	Result res;
	res.BPM = BPM;
	int start_time, next_time, input_time[10] = {0, }; // start_time: ���� ���� �ð�, next_time: ���� ��� �ð�, input_time: �Է� �޾��� ���� �ð� 
	int sumx = 0, sumx2 = 0; // ��հ� ǥ������ ����� ���� ���� 
	while(clock()%1000); // ù ��� �ð��� 1�� ������ ����� �ֱ� ���� ���� 
	next_time = clock();
	while(cnt < 10)
	{
		// ������ �������� ��Ʈ�γ� ����� ���� ���� 
		if(clock() >= next_time) {
			PlaySound("Metronome.wav", NULL, SND_ASYNC);
			next_time += t;
		} 
		// ������� �Է��� �޴� ���� 
		if(_kbhit()) {
			input_time[cnt] = clock() - sync;
			input = _getch();
			// Ư��Ű(����Ű Ȥ�� ���)�� �����ٸ� ���� ���� ������ 
			if(input == 224 || input == 0)
				_getch();
			// esc�� �����ٸ� ����� 
			else if(input == 27) {
				analysis();
				return;
			}
			// ���� ���� �ð� üũ(���ķ� ������ �� ����) 
			if(!cnt) {
				int delay = 0;
				while(input_time[cnt] - (next_time - t - delay) < 0) delay += t;
				
				if(input_time[cnt] - (next_time - t - delay) < (next_time - delay) - input_time[cnt]) 
					start_time = next_time - t - delay;
				else 
					start_time = next_time - delay;
			}
			
			// 0ms���� ���� ���� 
			res.devitation[cnt] = input_time[cnt] - start_time;
			printf("%4dms ", res.devitation[cnt]); 
			sumx += res.devitation[cnt];
			sumx2 += res.devitation[cnt]*res.devitation[cnt];
			start_time += t; 
			cnt++;
		}
	}
	// ���� �Ҹ� ��º��� ���� ������ �ȵ� �Ҹ� ��� 
	if(res.devitation[9] < 0) {
		while(clock() <= next_time);
		PlaySound("Metronome.wav", NULL, SND_ASYNC);
	} 
	
	// ��� �� ǥ������ ��� 
	res.avg = (double)sumx/10.0; res.sd = sqrt((double)sumx2/10.0 - res.avg*res.avg); // res.avg: ���, res.sd: ǥ������(sqrt(E(X^2) - (E(X))^2)) 
	printf("\n��� : %.1lfms, ǥ�� ���� : %.2lf\n", res.avg, res.sd);
	printf("\n��� ��������� ������� ��Ȯ ����(��40ms)���� ǥ�������� %s", res.sd <= 40 ? "�۽��ϴ�." : "Ů�ϴ�."); 
	save_load(2, res);
	
	current_pos = 25; 
	printf("\n����� �����Ͻðڽ��ϱ�? > ��     �ƴϿ�\n"); // ��: 25, �ƴϿ�: 32 
	printf("�ֱ� 10���� �����͸� ����ǹǷ� ����� ����� ���� �ֽ��ϴ�.");
	while(1)
	{
		if(_kbhit()) {
			input = _getch();
			// ����Ű ������ ���� 
			if(input == 224 || input == 0) {
				input = _getch();
				switch(input) {
					case 75: // ���� ����Ű 
						if(current_pos == 32) {
							arrowmove2(current_pos, 25, 14);
							current_pos = 25;
						}
						break;
					case 77: // ������ ����Ű 
						if(current_pos == 25) {
							arrowmove2(current_pos, 32, 14);
							current_pos = 32;
						}
						break;
				}
			}
			// ����Ű ���� 
			else if(input == 13) {
				switch(current_pos) {
					case 25: // ���� ���������� 
						save_load(1, res);
						gotoxy(25, 14);
						printf("����Ǿ����ϴ�.");
						Sleep(500);
						return; 
						break;
					case 32: // �ƴϿ��� ���������� 
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
	// ó�� �����Ͽ� ������ ���ٸ� ���� ���� 
	if(fp == NULL) {
		fp = fopen("users_record/users_record.csv", "w");
		fprintf(fp, "%d\n", 0);
		fclose(fp);
		fp = fopen("users_record/users_record.csv", "r");
	}
	char n; // n: ������ ���� 
	fscanf(fp, "%d", &n);
	Result data[10];
	// ������ �������� ������ 
	for(int i = 0; i < n; i++)
	{
		fscanf(fp, "%d", &data[i].BPM);
		for(int j = 0; j < 10; j++)
			fscanf(fp, "%d", &data[i].devitation[j]);
		fscanf(fp, "%lf %lf", &data[i].avg, &data[i].sd);
	}
	fclose(fp); 
	
	// ���� �м��� �� �� ���� ��ϰ� �񱳸� �ϴ°Ÿ� 
	if(mod == 2) {
		// �����Ͱ� ���ٸ� ���� 
		if(n == 0) {
			printf("\n���� ����Դϴ�.\n\n");
			return;
		}
		double sv_avg, sv_sd; // sv_avg: ����� ����� ����� ���, sv_sd: ����� ����� ����� ǥ������ 
		// ����� ����� ��հ� ǥ�������� ����� ���� 
		for(int i = 0; i < n; i++)
		{
			sv_avg += data[i].avg; sv_sd += data[i].sd;
		}
		sv_avg /= n; sv_sd /= n;
		// ��� �� 
		double avg_diff = fabs(res.avg) - fabs(sv_avg), sd_diff = res.sd - sv_sd;
		printf("\n����� ��� ��� %.1lfms���� 0ms���� ���̰� %s ", fabs(avg_diff), avg_diff > 0 ? "��������" : "�پ�����"); 
		printf("ǥ�������� %.2lf���� %s", fabs(sd_diff), sd_diff > 0 ? "�����Ͽ����ϴ�.\n" : "�����Ͽ����ϴ�.\n");
		if(avg_diff <= 0 && sd_diff <= 0) 
			printf("��հ� ǥ������ ��� ����� ��Ϻ��� �����ϴ�.\n");
		else if(avg_diff > 0 && sd_diff <= 0) 
			printf("����� ����� ��Ϻ��� 0ms���� ���̰� ũ���� ǥ�������� �� �����Ƿ� ���ڰ� �� �����մϴ�.\n"); 
		else if(avg_diff <= 0 && sd_diff > 0) 
			printf("����� ����� ��Ϻ��� 0ms���� ���̰� ������ ǥ�������� �� ũ�Ƿ� ���ڰ� �Ҿ����մϴ�.\n");
		else 
			printf("��հ� ǥ������ ��� ����� ��Ϻ��� �����մϴ�.\n");
	}
	// ������ �ϴ°Ÿ� 
	else if(mod == 1) {
		fp = fopen("users_record/users_record.csv", "w");
		// �����Ͱ� 10����� 
		if(n == 10) {
			fprintf(fp, "%d\n", n); // ���� ��ܿ� ������ ���� ��� 
			// �ֱ� 9���� �����͸� ��� 
			for(int i = 1; i < 10; i++)
			{
				fprintf(fp, "%d ", data[i].BPM);
				for(int j = 0; j < 10; j++)
					fprintf(fp, "%d ", data[i].devitation[j]);
				fprintf(fp, "%.1lf %.2lf\n", data[i].avg, data[i].sd);
			}
		}
		// �׺��� ���ٸ� 
		else {
			fprintf(fp, "%d\n", n+1); // ���� ��ܿ� ������ ���� ��� 
			for(int i = 0; i < n; i++) // �ֱ��� ��� ������ ��� 
			{
				fprintf(fp, "%d ", data[i].BPM);
				for(int j = 0; j < 10; j++)
					fprintf(fp, "%d ", data[i].devitation[j]);
				fprintf(fp, "%.1lf %.2lf\n", data[i].avg, data[i].sd);
			}
		}
		fclose(fp);
		// ������ ���� 
		fp = fopen("users_record/users_record.csv", "a");
		fprintf(fp, "%d ", res.BPM);
		for(int i = 0; i < 10; i++)
			fprintf(fp, "%d ", res.devitation[i]);
		fprintf(fp, "%.1lf %.2lf\n", res.avg, res.sd);
		fclose(fp);
	}
	// ���� ����� �ҷ� ���°Ÿ� 
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
	// ����� ���ٸ� 
	if(n == 0) {
		printf("����� �����ϴ�!");
		Sleep(1000);
		return false;
	} 
	// [1]���� [n]������ ��� ���� 
	printf(" > [1] %dBPM\n", data[0].BPM);
	for(int i = 1; i < n; i++)
		printf("   [%d] %dBPM\n", i+1, data[i].BPM);
	printf("   ����\n");
	
	unsigned char compare1; // compare1: ���� ������ �ε���
	current_pos = 0; 
	while(1)
	{
		// �Է��� �޾��� �� 
		if(_kbhit()) {
			input = _getch();
			// ����Ű ������ ���� 
			if(input == 224 || input == 0) {
				input = _getch();
				switch(input) {
					case 72: // �� ����Ű 
						if(current_pos > 0) {
							arrowmove1(current_pos, current_pos-1, 1);
							current_pos--;
						}
						else {
							arrowmove1(current_pos, n, 1);
							current_pos = n;
						}
						break;
					case 80: // �Ʒ� ����Ű 
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
			// ����Ű ���� 
			else if(input == 13) {
				if(current_pos == n) return false;
				compare1 = current_pos;
				system("cls");
				system("mode con:cols=95 lines=16");
				printf("BPM : %d\n", data[current_pos].BPM);
				printf("Ÿ�̹� : ��Ʈ�γ� ��� �Ҹ����� (+)�� ����, (-)�� ����\n  [1]");
				for(int i = 2; i <= 10; i++)
					printf("    [%d]", i);
				printf("\n");
				for(int i = 0; i < 10; i++)
					printf("%4dms ", data[current_pos].devitation[i]);
				printf("\n");
				printf("\n��� : %.1lfms, ǥ�� ���� : %.2lf\n", data[current_pos].avg, data[current_pos].sd);
				printf("\n > ���ϱ�   �ڷΰ���");
				current_pos = 1;
				while(1)
				{
					if(_kbhit()) {
						input = _getch();
						// ����Ű �Է� 
						if(input == 224 || input == 0) {
							input = _getch();
							switch(input) {
								case 75: // ���� ����Ű 
									if(current_pos == 12) {
										arrowmove2(current_pos, 1, 7);
										current_pos = 1;
									}
									break;
								case 77: // ������ ����Ű 
									if(current_pos == 1) {
										arrowmove2(current_pos, 12, 7);
										current_pos = 12;
									}
									break;
							}
						}
						// ����Ű �Է� 
						else if(input == 13) {
							switch(current_pos) {
								case 1: // ���ϱ⸦ ������ 
									return compare(n, data, compare1);
									break;
								case 12: // �ڷΰ��⸦ ������ 
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
	printf("���� ����� ����ּ���.\n");
	// [1] ���� [n]������ ��� ���� 
	printf(" > [1] %dBPM\n", data[0].BPM);
	for(int i = 1; i < n; i++)
		printf("   [%d] %dBPM\n", i+1, data[i].BPM);
	printf("   ����\n");
	
	gotoxy(2, 1);
	current_pos = 1;
	while(1)
	{
		// �Է��� �޾��� �� 
		if(_kbhit()) {
			input = _getch();
			// ����Ű ������ ���� 
			if(input == 224 || input == 0) {
				input = _getch();
				switch(input) {
					case 72: // �� ����Ű 
						if(current_pos > 1) {
							arrowmove1(current_pos, current_pos-1, 1);
							current_pos--;
						}
						else {
							arrowmove1(current_pos, n+1, 1);
							current_pos = n+1;
						}
						break;
					case 80: // �Ʒ� ����Ű 
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
			// ����Ű ���� 
			else if(input == 13) {
				if(current_pos == n+1) return false;
				current_pos--;
				if(current_pos == compare1) {
					printf(" ������ ����Դϴ�.");
					current_pos++;
					continue;
				}
				// ������ �� ����� ȭ�鿡 ǥ�� 
				system("cls");
				system("mode con:cols=150 lines=16");
				printf("BPM : %d", data[compare1].BPM);
				gotoxy(70, 0);
				printf("�� BPM : %d\n", data[current_pos].BPM);
				printf("Ÿ�̹� : ��Ʈ�γ� ��� �Ҹ����� (+)�� ����, (-)�� ����");
				gotoxy(70, 1);
				printf("��\n  [1]");
				for(int i = 2; i <= 10; i++)
					printf("    [%d]", i);
				printf(" ��  [1]");
				for(int i = 2; i <= 10; i++)
					printf("    [%d]", i);
				printf("\n");
				for(int i = 0; i < 10; i++)
					printf("%4dms ", data[compare1].devitation[i]);
				printf("��");
				for(int i = 0; i < 10; i++)
					printf("%4dms ", data[current_pos].devitation[i]);
				gotoxy(70, 4);
				printf("��");
				printf("\n��� : %.1lfms, ǥ�� ���� : %.2lf", data[compare1].avg, data[compare1].sd);
				gotoxy(70, 5);
				printf("�� ");
				printf("��� : %.1lfms, ǥ�� ���� : %.2lf\n", data[current_pos].avg, data[current_pos].sd);
				
				// ��� �� 
				double avg_diff = fabs(data[compare1].avg) - fabs(data[current_pos].avg), sd_diff = data[compare1].sd - data[current_pos].sd;
				printf("\n���ϴ� ��� ��� %.1lfms���� 0ms���� ���̰� %s ", fabs(avg_diff), avg_diff > 0 ? "��������" : "�پ�����"); 
				printf("ǥ�������� %.2lf���� %s", fabs(sd_diff), sd_diff > 0 ? "�����Ͽ����ϴ�.\n" : "�����Ͽ����ϴ�.\n");
				if(avg_diff < 0 && sd_diff < 0) 
					printf("��հ� ǥ������ ��� ���ϴ� ��Ϻ��� �����ϴ�.\n");
				else if(avg_diff > 0 && sd_diff < 0) 
					printf("����� ���ϴ� ��Ϻ��� 0ms���� ���̰� ũ���� ǥ�������� �� �����Ƿ� ���ڰ� �� �����մϴ�.\n"); 
				else if(avg_diff < 0 && sd_diff > 0) 
					printf("����� ���ϴ� ��Ϻ��� 0ms���� ���̰� ������ ǥ�������� �� ũ�Ƿ� ���ڰ� �Ҿ����մϴ�.\n");
				else 
					printf("��հ� ǥ������ ��� ���ϴ� ��Ϻ��� �����մϴ�.\n");
				printf("\n�ƹ�Ű�� ������ ��� ��ȸ ȭ������ ���ư��ϴ�.");
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
