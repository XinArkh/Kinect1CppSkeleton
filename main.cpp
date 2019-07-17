#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// kinect headers
#include "myKinect.h"
// console i/o, keyboard hitting detection
#include <conio.h>

int main() {
    // init kinect
    if (!initKinect()) {
        printf("Kinect initialization failed!");
        system("pause");
        exit(1);
    }

    // init file for saving angles
    FILE* fp;
    if ((fp = fopen("joints.txt", "w")) == NULL) {
        printf("Cannot open the .txt file!");
        system("pause");
        exit(1);
    }

    printf("Start tracking...\n");
    int n = 0;
    time_t t0, t;
    time(&t0);
    while (1) {
        bool captured = getSkeletalData();
        if (captured) {
            printf("Loop NO. %d: \n", n++);
            time(&t);
            fprintf(fp, "%ld", t - t0);
            for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; i++) {
                fprintf(fp, ",%f,%f,%f,%f", skeletonPosition[i].x, skeletonPosition[i].y, skeletonPosition[i].z, skeletonPosition[i].w);
            }
            fprintf(fp, "\n");
        }
        else {
            printf("Skeleton not captured.\n");
        }
        Sleep(100);
        if (_kbhit()) break;
    }
    fclose(fp);
    system("pause");
    return 0;
}
