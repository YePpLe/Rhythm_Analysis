#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MINUTE_MS 60000

void tick(long long tock);

int main(){
	int tempo;
	scanf("%d", &tempo);
	while(clock()<2000);
	long long sleepValue = MINUTE_MS / tempo;
	tick(sleepValue);
	return 0;
}

void tick(long long tock) {
	struct timespec ts;
	int fract = 1;
	ts.tv_sec = tock / 1000;
	ts.tv_nsec = (tock%1000) * 1000000;
	while(1) {
		printf("Tick! %d/4 %d\n", fract, clock());
		nanosleep(&ts, &ts);
		if(fract == 4) {
			fract = 0;
			printf("\n");
		}
		fract++;
	}
}
