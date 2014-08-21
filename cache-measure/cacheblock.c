/*
 *Cacheblock measures the block size of the CPU.
 *
 *@author: Von Quilon	@date: 12/10/2012
 */

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>

int main(void) {
	struct timeval start;
	struct timeval end;
	int i, power, times, dummy, stride, timeTaken;
	float percent;
	short int isConstant = 0;
	int prevTime = 0;
	int arraySize = 4096*4096;
	int loop = 10;
	volatile int *a;

	a = (int *)malloc(arraySize*sizeof(int));
	
	/*warms up the cache*/
	for(i=0; i<arraySize; i++)
		a[i] = 5;

	printf("\nProcessing...\n");

	/*checks from stride 1 byte to 4096 bytes
	 *but it will break out of the loop if the
	 *block size is found, so it will not check
	 *strides > block size
	 */
	for(power=0; power<11; power++) {
		stride = pow(2, power);
		gettimeofday(&start,NULL);
		for(times=0; times<loop; times++) {
			for(i=0; i<arraySize; i+=stride) {
				dummy = a[i];
			}
		}
		gettimeofday(&end,NULL);

		/*timeTaken is the time for execution in 
		 *micro seconds (usec)
		 */
		timeTaken = (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);
		if(prevTime != 0) {
			percent = (float)(prevTime-timeTaken)/prevTime;
			/*if there is a big drop after constant times
			 *block size is found and breaks out of the loop*/
			if(isConstant == 1) {
				if(percent>0.20)/*0.20 = 20%*/
					break;
			}
			else {
				/*if the time is now constant,
				 *sets isConstant variable to true
				 */
				if(percent<0.10)/*0.10 = 10%*/
					isConstant = 1;
			}
		}
		prevTime = timeTaken;
	}
	printf("Processor block size: %d bytes\n\n", stride*2);
	free(a);
	return 0;
}
