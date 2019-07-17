#pragma once
// Ole2.h and Windows.h should be included for the Kinect includes to work correctly
#include <Windows.h>
#include <Ole2.h>
// Kinect includes
#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

// Kinect variables
extern HANDLE rgbStream;
extern INuiSensor* sensor;
// Stores the coordinates of each joint
extern Vector4 skeletonPosition[NUI_SKELETON_POSITION_COUNT];
extern double angles[8];

bool initKinect();
bool getSkeletalData();