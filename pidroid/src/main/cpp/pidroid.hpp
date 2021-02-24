//
// Created by adrian on 23/02/21.
//
#include <jni.h>

namespace pidroidlib {
    static struct {
        jclass classRef;
        jfieldID minsize;
        jfieldID maxsize;
        jfieldID angle;
        jfieldID scalefactor;
        jfieldID stridefactorWidth;
        jfieldID stridefactorHeight;
        jfieldID qthreshold;
        jfieldID perturbs;
        jfieldID clustering;
        jfieldID pupilDetectionEnable;
        jfieldID landmarkDetectionEnable;
        jfieldID prominentFaceOnly;
    } jPidroidConfig;


    static struct {
        jclass classRef;
        jmethodID init;
        jfieldID topLeft;
        jfieldID width;
        jfieldID height;
        jfieldID eyes;
        jfieldID landmarks;
    } jFace;


    static struct {
        jclass classRef;
        jmethodID init;
        jmethodID size;
        jmethodID add;
        jmethodID get;
    } jArrayList;

    static struct {
        jclass classRef;
        jfieldID faces;
        jfieldID detected;
    } jFaceDetectionResult;

    static struct {
        jclass classRef;
        jfieldID x;
        jfieldID y;
    } jPoint;

    static struct {
        jclass classRef;
        jfieldID center;
        jfieldID radius;
        jmethodID init;
    } jEye;

    static struct {
        jclass classRef;
        jfieldID center;
        jfieldID radius;
        jmethodID init;
    } jLandmark;

    class Pidroid {
    private:
        Puploc puploc1;
        Pico picol;
        Flploc flploc;

        bool pupilDetectionEnable = false;
        bool landmarkDetectionEnable = false;
        bool prominentFaceOnly = false;
        bool clustering = true;
        int perturbs = 50;

        static std::vector<jobject> arraylistToVectorJObject(JNIEnv *env, jobject arrayList);
        static uint8_t* RGBABytesToGrayBytes(JNIEnv *env, const uint8_t* rgbaBytes, int width, int height);
        static jobject vectorJObjectToArrayList(JNIEnv *env, std::vector<jobject> vector);
        static std::vector<char> readFileFromAssets(JNIEnv *pEnv, AAssetManager *mgr,const char *folder, const char *filename);
    public:
        Pidroid() = default;
        ~Pidroid() = default;
        static int loadClasses(JNIEnv *env);
        void setup(JNIEnv *env, jobject thiz, jobject pidroidConfig, jobject assetManager);
        void detectFace(JNIEnv *env, jobject thiz, jintArray rgbaBytes, jint width,  jint height, jobject detection_info);
    };
}
