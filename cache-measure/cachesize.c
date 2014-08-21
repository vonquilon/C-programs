/*
 *Cachesize measures the cache size of the CPU
 *from the L1 cache to the L3 cache.
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
	int i, power, dummy, lengthMod, arraySize, timeTaken;
	float percent, minPercent;
	int cachesize;
	int steps = 64*1024*1024;
	int prevTime = 0;
	short int level = 1;
	short int isConstant = 0;
	short int isPrinted = 1;
	register *a;

	a=sbrk(4*2048*4096+8192);
	a=(int *)((((int)a>>13)<<13)+8192);

	printf("\nProcessing...\n");

	/*checks from array size of 1KB to 32MB*/
	for(power=8; power<24; power++) {
		arraySize = pow(2, power);
		lengthMod = arraySize - 1;
		
		/*warms up the cache*/
		for(i=0; i<arraySize; i++)
			a[i] = 5;
	
		gettimeofday(&start,NULL);
		/*traverses every 16th element of the array,
		 *reaches the end of the array, and starts back
		 *to the beginning until the loop ends*/
		for (i=0; i<steps; i++)
			dummy = a[(i*16)&lengthMod];
		gettimeofday(&end,NULL);

		/*timeTaken is the time for execution in
		 *micro seconds (usec)
		 */
		timeTaken = (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);
		if(prevTime != 0) {
			percent = (float)(timeTaken-prevTime)/prevTime;
			if(level >= 3)
				minPercent = 0.30;
			else
				minPercent = 0.019;
			/*if there is a big jump in time, then the
			 *cache size is found only if the times after it
			 *are constant
			 */
			if(percent > minPercent) {
				cachesize = ((arraySize*4)/1024)/2;
				isConstant = 0;
				isPrinted = 0;
			}
			else
				isConstant = 1;
			/*if the times after the found cache size are
			 *constant, then the cache size is printed out,
			 *level is incremented, and isPrinted is set
			 *to true
			 */
			if(isConstant == 1 && isPrinted == 0) {
				printf("L%d cachesize: %dK\n", level, cachesize);
				level++;
				isPrinted = 1;
			}
		}
		prevTime = timeTaken;
	}
	/*if the L3 cache is not found*/
	if(level == 3) {
		printf("L%d cachesize: N/A\n", level);
	}
	printf("\n");
	return 0;
}
