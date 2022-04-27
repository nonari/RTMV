#include "worker.h"
#include "configuration.h"

// Declare stable frame(s) here
// static uchar pix[W*H];

Worker::Worker(QObject *parent) : QThread(parent) {
    std::cout << "Init worker..." << std::endl;
    // Init RNG
    RandGen=std::mt19937(rd());
    RandDist=std::uniform_int_distribution<int>(0,99);

    // Init (and configure) cameras here
    HaveCams = InitCams();
}


Worker::~Worker()
{
    // In (strange) case we are closing the shop with camera still streaming
    GoOn = 0;
    // May wait here for a few ms, to give time for cam to stop grabbing,
    // so freeing resources will not cause an error on exit
    sleep(10);
    // Release any resources here

    // Close cameras here
    CloseCams();
    // if(HaveCams) ...
}

void Worker::run() {
    // StartStreaming here
    //StartStreamin();

    switch(Mode) {
        case 0:
            break;
        case 1:
            DoDaWork();
            break;
    }

    // StopStreaming here
    GoOn = 0;
    CloseCams();
}

void Worker::Grab(){
    std::cout << "Grab()" << std::endl;
    try {
//        camera->MaxNumBuffer = 5;
//        int c_countOfImagesToGrab = 1;
//        camera->StartGrabbing(c_countOfImagesToGrab);

        Pylon::CGrabResultPtr ptrGrabResult;

        camera->GrabOne(50, ptrGrabResult);
        const uint8_t* pImageBuffer = (uint8_t*) ptrGrabResult->GetBuffer();
        std::cout << "Gray value of first pixel: " << (uint32_t) pImageBuffer[500] << std::endl << std::endl;
        auto im0 = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetHeight(), CV_8U, *pImageBuffer);
        std::cout << "height: " << ptrGrabResult->GetHeight() << std::endl;
        cv::imwrite("gaura.png", im0);
        //        cv::namedWindow("hola");
//        cv::imshow("hola", im0);
//        cv::waitKey(5000);
    } catch(const Pylon::GenericException& e){
        std::cerr << e.what();
    } catch(const std::exception& e){
        std::cerr << e.what();
    }
}

void Worker::DoDaWork()
{
    nhits = 0;

    try {
        // Declare grab result here
        while (GoOn) {

            // Wait for all cameras here

            int co = 0;
            while (RandDist(RandGen)!=35) {
                // Random gen used here to simulate grab wait and timeouts
                // In actual application, wait for grab success instead
                msleep(50);
                Grab();
                if (!GoOn) {return;}
                if(++co > 59) {emit TimeOut(); co=0;}
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
        Kaboum = 1;
        GoOn = 0;
        return;
    }
}



// ------------------- Init camera(s) and grabber(s) -------------------------
bool Worker::InitCams(void)
{
    try {
        Pylon::PylonInitialize();

        // Do whatever to initialize connection
        camera = new Pylon::CInstantCamera(Pylon::CTlFactory::GetInstance().CreateFirstDevice());
        std::cout << "Using device: " << camera->GetDeviceInfo().GetModelName() << std::endl;

        ConfigCams();
        GenApi::INodeMap& nodemap = camera->GetNodeMap();
        // Select test image 1
        Pylon::CEnumParameter(nodemap, "TestImageSelector").SetValue("Off");
        // Enable custom test images
        Pylon::CEnumParameter(nodemap, "ImageFileMode").SetValue("On");
        // Load custom test image from disk
        Pylon::CStringParameter(nodemap, "ImageFilename").SetValue("/home/nonari/cara/BN_15_1.png");        // ...

        // Sometimes it's convenient to wait a little for the hardware to warm up
        // before connection can be established, so we wait and retry a few times

        int c=0;  // Wait for cam máx 30 secs.
//patience:
//        try {
//            while(CameraNotResponding) {
//                std::this_thread::sleep_for(std::chrono::seconds(1));
//                c++; if(c>30) return false;
//            }
//        } catch (const Pylon::GenericException& e) {
//            std::this_thread::sleep_for(std::chrono::seconds(1));
//            c++; if(c>30) return false;
//            goto patience;
//        } catch (...) {
//            std::this_thread::sleep_for(std::chrono::seconds(1));
//            c++; if(c>30) return false;
//            goto patience;
//        }


        // Cam successfully connected, set up the rest of things

        // Configure the cameras, if needed (normally, the proper config is already
        // saved into the cam) before starting any streaming

        // Let camera(s) ready to begin streaming

    } catch(const Pylon::GenericException& e) {
        std::cerr << e.what();
        return false;
    } catch(...) {
        return false;
    }

    return true;
}


// ------------------------------- Init grab -------------------------------
bool Worker::StartStreamin(void) {
    try {
        // Camera(s) start streaming
        camera->StartGrabbing();
    } catch(const Pylon::GenericException& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}


// ---------------------- Release camera resources -------------------------
void Worker::CloseCams(void) {
    try {
        // Free all resources
        if (camera->IsGrabbing()) {
            camera->StopGrabbing();
        }
        camera->Close();
        camera->DestroyDevice();
        delete camera;
        camera = nullptr;

    } catch(const Pylon::GenericException& e) {
          std::cerr << e.what() << std::endl;
//        QMessageBox msgBox;
//        QString msg(e.what());
//        msgBox.setText(msg);
//        msgBox.exec();
    }
}

// ---------------------- Config camera -------------------------

void Worker::ConfigCams(void) {
    camera->RegisterConfiguration(new Configuration(), Pylon::RegistrationMode_Append, Pylon::Cleanup_Delete);
    camera->Open();
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




