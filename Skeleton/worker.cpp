#include "worker.h"

// Declare stable frame(s) here
// static uchar pix[W*H];


Worker::Worker(QObject *parent) : QThread(parent)
{
    // Init RNG
     RandGen=std::mt19937(rd());
     RandDist=std::uniform_int_distribution<int>(0,99);

     // Init (and configure) cameras here

     // HaveCams=InitCams();
}


Worker::~Worker()
{
    // In (strange) case we are closing the shop with camera still streaming
    GoOn=0;
    // May wait here for a few ms, to give time for cam to stop grabbing,
    // so freeing resources will not cause an error on exit

    // Release any resources here

    // Close cameras here

    // if(HaveCams) ...
}

void Worker::run()
{
    // StartStreaming here

    switch(Mode) {
        case 0:
            break;
        case 1:
            DoDaWork();
            break;
    }

    // StopStreaming here

    GoOn=0;
}

void Worker::Grab(){
    try {
        CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice() );
        cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl;

        camera.MaxNumBuffer = 5;
        int c_countOfImagesToGrab = 1;
        camera.StartGrabbing(c_countOfImagesToGrab);

        CGrabResultPtr ptrGrabResult;

        while (camera.IsGrabbing()) {
            camera.RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException );

            if (ptrGrabResult->GrabSucceeded())
            {
                cout << "SizeX: " << ptrGrabResult->GetWidth() << endl;
                cout << "SizeY: " << ptrGrabResult->GetHeight() << endl;
                const uint8_t* pImageBuffer = (uint8_t*) ptrGrabResult->GetBuffer();
                cout << "Gray value of first pixel: " << (uint32_t) pImageBuffer[0] << endl << endl;
            }}}catch(const GenericException& e){


    }
}

void Worker::DoDaWork()
{
    nhits=0;

    try {

        // Declare grab result here

        while(GoOn) {

            // Wait for all cameras here

            int co=0;
            while(RandDist(RandGen)!=35) {
                // Random gen used here to simulate grab wait and timeouts
                // In actual application, wait for grab success instead
                msleep(50);
                Grab();
                if(!GoOn) {return;}
                if(++co>59) {emit TimeOut(); co=0;}
            }

            // For all cameras:
                // Get frame(s) here from grab result
                // Copy frame(s) to stable buffer
                // Re-enqueue frame(s) for next grab

            // Process stable frame(s)

            // Pass to interface whatever results or indicators to show in real time

            emit Result(++nhits);

        }

    } catch(...) {
        // Use Kaboum to signal the interface the type of problem that happened
        Kaboum=1;
        GoOn=0;
        return;
    }
}


/*
// ------------------- Init camera(s) and grabber(s) -------------------------
bool Worker::InitCams(void)
{
    try {

        // Do whatever to initialize connection

        // ...

        // Sometimes it's convenient to wait a little for the hardware to warm up
        // before connection can be established, so we wait and retry a few times

        int c=0;  // Wait for cam máx 30 secs.
patience:
        try {
            while(CameraNotResponding) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                c++; if(c>30) return false;
            }
        } catch (const Pylon::GenericException& e) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            c++; if(c>30) return false;
            goto patience;
        } catch (...) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            c++; if(c>30) return false;
            goto patience;
        }


        // Cam successfully connected, set up the rest of things

        // Configure the cameras, if needed (normally, the proper config is already
        // saved into the cam) before starting any streaming

        // Let camera(s) ready to begin streaming

    } catch(const Pylon::GenericException& e) {
        return false;
    } catch(...) {
        return false;
    }

    return true;
}


// ------------------------------- Init grab -------------------------------
bool Worker::StartStreamin(void)
{

    try {

        // Camera(s) start streaming

    } catch(const Pylon::GenericException& e) {
        return false;
    }

    return true;
}


// ---------------------- Release camera resources -------------------------
void Worker::CloseCams(void)
{
    try {

        // Free all resources

    } catch(const Pylon::GenericException& e) {
        QMessageBox msgBox;
        QString msg(e.what());
        msgBox.setText(msg);
        msgBox.exec();
    }
}

// ---------------------- Config camera -------------------------

void Worker::ConfigCams(void) {

    // Pass whatever config parameters needed by the cam(s)

    // Setting up the config with PylonWiever beforehand
    // and storing it into the cam(s) for autoload at startup
    // is recommended, except those parameters that may need
    // adjusting upon specific varying circumstances (if they
    // exist in the actual application) at startup time of every
    // session (e.g. a mobile machine used in different environments
    // of for different kind of targets every time, or e.g. a fixed
    // machine in a production line with different working conditions
    // in different sessions (belt speed, amount of light, etc.)

}

*/


