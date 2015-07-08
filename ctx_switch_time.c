#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE	1024*1024*10
#define TIME_DIFF_SIZE	100*1000

int
main(int argc, char **argv)
{
	struct timespec tres;
	int i;
	int j;
	char buffer[BUFFER_SIZE];
	struct timespec tdiffs[TIME_DIFF_SIZE];
	double smallavg = 0;
	double bigavg = 0;
	unsigned long small = 6000;
	unsigned long bfactor = 12;
	unsigned long big_dev = 1000;

	(void)argc;
	(void)argv;

#define C(t) do {					\
	memset(&tres, 0, sizeof(tres));			\
	i = clock_getres((t), &tres);			\
	assert(i == 0);					\
	printf("--- %s ---\n", (#t));			\
	printf(" sec: %ld\n", (long int)tres.tv_sec);	\
	printf("nsec: %ld\n", (long int)tres.tv_nsec);	\
} while (0)

	C(CLOCK_REALTIME);
	C(CLOCK_MONOTONIC);
	C(CLOCK_UPTIME);
	C(CLOCK_VIRTUAL);

	memset(tdiffs, 0, sizeof(tdiffs));
	fflush(stdout);
	j = setvbuf(stdout, buffer, _IOFBF, sizeof(buffer));
	assert(j == 0);

	for (i = 1; i < TN; i++) {
		clock_gettime(CLOCK_MONOTONIC, &tdiffs[i]);
		tdiffs[i - 1].tv_sec =
		    tdiffs[i].tv_sec - tdiffs[i - 1].tv_sec;
		tdiffs[i - 1].tv_nsec =
		    tdiffs[i].tv_nsec - tdiffs[i - 1].tv_nsec;
	}

	unsigned long sec = 0;
	unsigned long nsec = 0;
	unsigned long big = small * bfactor;
	int smalln = 0;
	int bign = 0;

	for (i = 10; i < TN; i++) {
		sec = tdiffs[i].tv_sec;
		nsec = tdiffs[i].tv_nsec;

		if (nsec < big - big_dev) {
			smallavg += nsec;
			smalln++;
		} else {
			bigavg += nsec;
			bign++;
		}
		printf("%u %li\n", tdiffs[i].tv_sec, tdiffs[i].tv_nsec);
	}
	printf("bign=%d\n", bign);
	bigavg /= bign;
	smallavg /= smalln;

	printf("bigavg: %lf smallavg: %lf\n", bigavg, smallavg);

	exit(0);
}
