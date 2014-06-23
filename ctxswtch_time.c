#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int
main(int argc, char **argv)
{
	struct timespec tres;
	int i;
	int j;
#define NBUF 1024*1024*10
	char buffer[NBUF];
#define TN 100*1000
	struct timespec tdiffs[TN];
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

#if 0

zsh: command not found: CAT
     int
     clock_getres(clockid_t clock_id, struct timespec *tp);

DESCRIPTION
     The clock_gettime() and clock_settime() allow the calling process to
     retrieve or set the value used by a clock which is specified by clock_id.

     The clock_id argument can be one of five values: CLOCK_REALTIME for time
     that increments as a wall clock should, CLOCK_MONOTONIC which increments
     in SI seconds, CLOCK_UPTIME which starts at zero when the kernel boots
     and increments monotonically in SI seconds while the machine is running,
     CLOCK_VIRTUAL for time that increments only when the CPU is running in
     user mode on behalf of the calling process, or CLOCK_PROF for time that
     increments when the CPU is running in user or kernel mode.

     The structure pointed to by tp is defined in <sys/timespec.h> as:

     struct timespec {
             time_t  tv_sec;         /* seconds */
             long    tv_nsec;        /* and nanoseconds */
     };

     Only the super-user may set the time of day.  If the system securelevel
     is greater than 1 (see init(8)), the time may only be advanced.  This
     limitation is imposed to prevent a malicious super-user from setting
     arbitrary time stamps on files.  The system time can still be adjusted
     backwards using the adjtime(2) system call even when the system is
     secure.

     The resolution (granularity) of a clock is returned by the clock_getres()
     system call.  This value is placed in a (non-NULL) *tp.
#endif
