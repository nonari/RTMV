#ifndef WORKER_H
#define WORKER_H

#include <QThread>

// Includes for whatever you need from OpenCV and Pylon, e.g.
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <pylon/PylonIncludes.h>
#include <pylon/gige/PylonGigEIncludes.h>
#include <pylon/gige/BaslerGigECamera.h>


#include <random>
#include <vector>

#include "globals.h"


class Worker : public QThread {
    Q_OBJECT

public:
    Worker(QObject *parent = nullptr);
    ~Worker();

signals:
    void Result(const int& N);
    void TimeOut(void);

private:
    void run(void);
    void DoDaWork(void);
    void Grab(void);

    std::vector<int> g1;
    std::random_device rd;
    std::mt19937 RandGen;
    std::uniform_int_distribution<int> RandDist;

    int nhits;


    // CAMERA RESOURCES, e.g. with older Basler SDK versions:
    /*
    Pylon::PylonAutoInitTerm autoInitTerm;
    Camera Cam[NUM_DEVICES];
    Camera::StreamGrabber_t Grab[NUM_DEVICES];
    unsigned char buffers[NUM_DEVICES][NUM_BUFFERS][WW*HH];         // Buffers used for grabbing
    Pylon::StreamBufferHandle bufHandles[NUM_DEVICES][NUM_BUFFERS]; // Handles for the buffers
    Pylon::WaitObjects wos;                                         // Waiting objects handle
    int Context[NUM_BUFFERS];
    */
    Pylon::CInstantCamera *camera;

    // CAMERA HANDLING FUNCTIONS, e.g.

    bool InitCams(void);
    void CloseCams(void);
    bool StartStreamin(void);
    void ConfigCams(void);

};

#endif // WORKER_H
