#include <stdio.h>
#include "myKinect.h"

// Kinect variables
HANDLE rgbStream;
INuiSensor* sensor;
// Stores the coordinates of each joint
Vector4 skeletonPosition[NUI_SKELETON_POSITION_COUNT];

bool initKinect() {
    printf("Initializing Kinect...\n");
    // Get a working kinect sensor
    int numSensors;
    if (NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) {
        printf("Can't find device!\n");
        system("pause");
        return false;
    }
    if (NuiCreateSensorByIndex(0, &sensor) < 0) {
        printf("Can't create sensor!\n");
        return false;
    }

    // Initialize sensor
    sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_SKELETON);
    sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, // Depth camera or rgb camera?
        NUI_IMAGE_RESOLUTION_640x480,                // Image resolution
        0,      // Image stream flags, e.g. near mode
        2,      // Number of frames to buffer
        NULL,   // Event handle
        &rgbStream);
    sensor->NuiSkeletonTrackingEnable(NULL, NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT);
    // NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT for only upper body
    printf("Initialization done.\n");
    return sensor;
}

bool getSkeletalData() {
    bool captured = false;
    NUI_SKELETON_FRAME skeletonFrame = { 0 };
    if (sensor->NuiSkeletonGetNextFrame(0, &skeletonFrame) >= 0) {
        sensor->NuiTransformSmooth(&skeletonFrame, NULL);
        // Loop over all sensed skeletons to find the first captured one
        for (int z = 0; z < NUI_SKELETON_COUNT; ++z) {
            const NUI_SKELETON_DATA& skeleton = skeletonFrame.SkeletonData[z];
            // Check the state of the skeleton
            if (skeleton.eTrackingState == NUI_SKELETON_TRACKED) {
                captured = true;
                // Copy the joint positions into our array
                for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; ++i) {
                    skeletonPosition[i] = skeleton.SkeletonPositions[i];
                    if (skeleton.eSkeletonPositionTrackingState[i] ==
                        NUI_SKELETON_POSITION_NOT_TRACKED) {
                        skeletonPosition[i].w = 0;
                    }
                }
                break;
            }
        }
    }
    return captured;
}