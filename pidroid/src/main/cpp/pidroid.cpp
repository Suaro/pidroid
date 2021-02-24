
/*
 * See the file "LICENSE.md" for the full license governing this code.
 */

#include <jni.h>
#include <android/log.h>
#include <cstring>
#include <vector>
#include <string>
#include "pidroid/puploc.hpp"
#include "pidroid/flploc.hpp"
#include "pidroid/pico.hpp"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <chrono>
#include <thread>
#include "pidroid.hpp"

#define DEBUG_TAG "pidroid"



pidroidlib::Pidroid pidroid;



extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {

    JNIEnv *env;
    jint status = vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (status != JNI_OK)
        return -1;

    int result = pidroidlib::Pidroid::loadClasses(env);

    if(result != 0) {
        return result;
    }

    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_suaro_pidroid_core_NativeMethods_00024Companion_setup(JNIEnv *env, jobject thiz, jobject pidroidConfig, jobject assetManager) {

   pidroid = pidroidlib::Pidroid();
   pidroid.setup(env, thiz, pidroidConfig, assetManager);

}


extern "C"
JNIEXPORT void JNICALL
Java_com_suaro_pidroid_core_NativeMethods_00024Companion_detectFace(JNIEnv *env, jobject thiz,
                                                                    jintArray rgbaBytes, jint width,
                                                                    jint height,
                                                                    jobject detection_info) {

    pidroid.detectFace(env, thiz, rgbaBytes, width, height, detection_info);

}

uint8_t* pidroidlib::Pidroid::RGBABytesToGrayBytes(JNIEnv *env, const uint8_t* rgbaBytes, int width, int height) {
    const int size = height * width;
    int i = -1;
    float b = 0, g = 0, r = 0, gray = 0;
    auto * out = new uint8_t[size];

    for(i = 0; i < size; ++i ) {
        b =  rgbaBytes[i * 4 + 2];
        g = rgbaBytes[i * 4 + 1];
        r = rgbaBytes[i * 4 + 0];
        gray = 0.2f* r + 0.7f*g + 0.1f*b;
        out[i] = int(gray);
    }
    return out;
}

std::vector<jobject> pidroidlib::Pidroid::arraylistToVectorJObject(JNIEnv *env, jobject arrayList) {
    jint len = env->CallIntMethod(arrayList, jArrayList.size);
    std::vector<jobject> result;
    result.reserve(len);
    for (jint i=0; i<len; i++) {
        auto element = static_cast<jobject>(env->CallObjectMethod(arrayList, jArrayList.get, i));
        result.emplace_back(element);
        env->DeleteLocalRef(element);
    }
    return result;
}

jobject pidroidlib::Pidroid::vectorJObjectToArrayList(JNIEnv *env, std::vector<jobject> vector) {
    jobject result = env->NewObject(jArrayList.classRef, jArrayList.init, vector.size());
    for (jobject element: vector) {
        env->CallBooleanMethod(result, jArrayList.add, element);
        env->DeleteLocalRef(element);
    }
    return result;
}

std::vector<char> pidroidlib::Pidroid::readFileFromAssets(JNIEnv *pEnv, AAssetManager *mgr, const char *folder, const char *filename) {
    AAssetDir* assetDir = AAssetManager_openDir(mgr, folder);
    const char* currentFilename;
    std::vector<char> buffer;
    while ((currentFilename = AAssetDir_getNextFileName(assetDir)) != NULL)
    {
        //search for desired file
        if(!strcmp(currentFilename, filename))
        {
            AAsset *asset = AAssetManager_open(mgr, currentFilename, 2);

            //holds size of searched file
            off64_t length = AAsset_getLength64(asset);
            //keeps track of remaining bytes to read
            off64_t remaining = AAsset_getRemainingLength64(asset);
            size_t Mb = 1000 *1024; // 1Mb is maximum chunk size for compressed assets
            size_t currChunk;
            buffer.reserve(length);

            //while we have still some data to read
            while (remaining != 0)
            {
                //set proper size for our next chunk
                if(remaining >= Mb)
                {
                    currChunk = Mb;
                }
                else
                {
                    currChunk = remaining;
                }
                char chunk[currChunk];

                //read data chunk
                if(AAsset_read(asset, chunk, currChunk) > 0) // returns less than 0 on error
                {
                    //and append it to our vector
                    buffer.insert(buffer.end(),chunk, chunk + currChunk);
                    remaining = AAsset_getRemainingLength64(asset);
                }
            }
            AAsset_close(asset);
        }

    }

    return buffer;
}


int pidroidlib::Pidroid::loadClasses(JNIEnv *env) {

    jclass pointClass = env->FindClass("com/suaro/pidroid/core/Point");
    if (pointClass == NULL) {
        return -1;
    }
    jPoint.classRef = (jclass) env->NewGlobalRef(pointClass);
    jPoint.x = env->GetFieldID(pointClass, "x", "I");
    jPoint.y = env->GetFieldID(pointClass, "y", "I");

    jclass dEyeClass = env->FindClass("com/suaro/pidroid/core/Eye");
    if (dEyeClass == NULL) {
        return -1;
    }

    jEye.classRef = (jclass) env->NewGlobalRef(dEyeClass);
    jEye.init     = env->GetMethodID(dEyeClass, "<init>", "()V");
    jEye.center = env->GetFieldID(dEyeClass, "center", "Lcom/suaro/pidroid/core/Point;");
    jEye.radius = env->GetFieldID(dEyeClass, "radius", "I");

    jclass dLandmarkClass = env->FindClass("com/suaro/pidroid/core/Landmark");
    if (dLandmarkClass == NULL) {
        return -1;
    }

    jLandmark.classRef = (jclass) env->NewGlobalRef(dLandmarkClass);
    jLandmark.init     = env->GetMethodID(dLandmarkClass, "<init>", "()V");
    jLandmark.center = env->GetFieldID(dLandmarkClass, "center", "Lcom/suaro/pidroid/core/Point;");
    jLandmark.radius = env->GetFieldID(dLandmarkClass, "radius", "I");

    jclass dFaceClass = env->FindClass("com/suaro/pidroid/core/Face");
    if (dFaceClass == NULL) {
        return -1;
    }
    jFace.classRef =  (jclass) env->NewGlobalRef(dFaceClass);
    jFace.init     = env->GetMethodID(dFaceClass, "<init>", "()V");
    jFace.topLeft = env->GetFieldID(dFaceClass, "topLeft",
                                    "Lcom/suaro/pidroid/core/Point;");
    jFace.width = env->GetFieldID(dFaceClass, "width",
                                  "I");
    jFace.height = env->GetFieldID(dFaceClass, "height",
                                   "I");
    jFace.eyes = env->GetFieldID(dFaceClass, "eyes", "Ljava/util/ArrayList;");

    jFace.landmarks = env->GetFieldID(dFaceClass, "landmarks", "Ljava/util/ArrayList;");

    jclass dArrayList = env->FindClass("java/util/ArrayList");
    if (dArrayList == NULL) {
        return -1;
    }
    jArrayList.classRef = (jclass) env->NewGlobalRef(dArrayList);
    jArrayList.init     = env->GetMethodID(dArrayList, "<init>", "(I)V");
    jArrayList.size = env->GetMethodID (dArrayList, "size", "()I");
    jArrayList.get  = env->GetMethodID(dArrayList, "get", "(I)Ljava/lang/Object;");
    jArrayList.add  = env->GetMethodID(dArrayList, "add", "(Ljava/lang/Object;)Z");

    jclass dInfoClass = env->FindClass("com/suaro/pidroid/core/FaceDetectionResult");
    if (dInfoClass == NULL) {
        return -1;
    }
    jFaceDetectionResult.classRef = (jclass) env->NewGlobalRef(dInfoClass);
    jFaceDetectionResult.faces = env->GetFieldID(dInfoClass, "faces", "Ljava/util/ArrayList;");
    jFaceDetectionResult.detected = env->GetFieldID(dInfoClass, "detected", "Z");

    jclass dCascadeClass = env->FindClass("com/suaro/pidroid/core/PidroidConfig");
    if (dCascadeClass == NULL) {
        return -1;
    }
    jPidroidConfig.classRef = (jclass) env->NewGlobalRef(dCascadeClass);
    //jPidroidConfig.angle = env->GetFieldID(dCascadeClass, "angle", "F");
    jPidroidConfig.maxsize = env->GetFieldID(dCascadeClass, "maxsize", "I");
    jPidroidConfig.minsize = env->GetFieldID(dCascadeClass, "minsize", "I");
    jPidroidConfig.perturbs = env->GetFieldID(dCascadeClass, "perturbs", "I");
    jPidroidConfig.clustering = env->GetFieldID(dCascadeClass, "clustering", "Z");
    jPidroidConfig.qthreshold = env->GetFieldID(dCascadeClass, "qthreshold", "F");
    jPidroidConfig.scalefactor = env->GetFieldID(dCascadeClass, "scalefactor", "F");
    jPidroidConfig.stridefactor = env->GetFieldID(dCascadeClass, "stridefactor", "F");
    jPidroidConfig.pupilDetectionEnable = env->GetFieldID(dCascadeClass, "pupilDetectionEnable", "Z");
    jPidroidConfig.landmarkDetectionEnable = env->GetFieldID(dCascadeClass, "landmarkDetectionEnable", "Z");
    jPidroidConfig.prominentFaceOnly = env->GetFieldID(dCascadeClass, "prominentFaceOnly", "Z");

    return 0;
}



void
pidroidlib::Pidroid::setup(JNIEnv *env, jobject thiz, jobject pidroidConfig, jobject assetManager) {
    pidroidlib::CascadeParams cascadeParams = {
            .minSize = env->GetIntField(pidroidConfig, jPidroidConfig.minsize),
            .maxSize = env->GetIntField(pidroidConfig, jPidroidConfig.maxsize),
            .shiftFactor = env->GetFloatField(pidroidConfig, jPidroidConfig.stridefactor),
            .scaleFactor = env->GetFloatField(pidroidConfig, jPidroidConfig.scalefactor),
            //.angle = env->GetFloatField(pidroidConfig, jPidroidConfig.angle),
            .angle = 0,
            .minThreshold = env->GetFloatField(pidroidConfig, jPidroidConfig.qthreshold)
    };

    perturbs = env->GetIntField(pidroidConfig, jPidroidConfig.perturbs);
    clustering = env->GetBooleanField(pidroidConfig, jPidroidConfig.clustering);
    pupilDetectionEnable = env->GetBooleanField(pidroidConfig, jPidroidConfig.pupilDetectionEnable);
    landmarkDetectionEnable = env->GetBooleanField(pidroidConfig, jPidroidConfig.landmarkDetectionEnable);
    prominentFaceOnly = env->GetBooleanField(pidroidConfig, jPidroidConfig.prominentFaceOnly);

    AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);

    std::vector<char> facefinder = readFileFromAssets(env, mgr, "", "facefinder");
    std::vector<char> puploc = readFileFromAssets(env, mgr, "", "puploc");

    picol = pidroidlib::Pico(cascadeParams);
    picol.unpackPicoCascade(facefinder);

    puploc1 = pidroidlib::Puploc();
    puploc1.unpackCascade(puploc);

    //TODO: Improve flp reads.
    std::vector<std::string> flpCascades = {"lp38", "lp42", "lp44",
                                            "lp46", "lp81", "lp82",
                                            "lp84", "lp93", "lp312"};

    for(const auto& cascadeName : flpCascades) {
        flploc.addCascade(cascadeName, readFileFromAssets(env, mgr, "", cascadeName.c_str()));
    }
}

void pidroidlib::Pidroid::detectFace(JNIEnv *env, jobject thiz, jintArray rgbaBytes, jint width,
                                     jint height, jobject detection_info) {

    uint8_t* pixels;
    uint8_t* rgbaPixels;
    int nrows, ncols, ldim;


    std::vector<jobject> faceArray = std::vector<jobject>();
    std::vector<jobject> eyeArray = std::vector<jobject>();
    std::vector<jobject> landmarkArray = std::vector<jobject>();

    nrows = height;
    ncols = width;
    ldim = width;

    jint* dataPtr = (*env).GetIntArrayElements(rgbaBytes, NULL);

    rgbaPixels = (uint8_t*)dataPtr;
    pixels = RGBABytesToGrayBytes(env, rgbaPixels, ncols, nrows);

    pidroidlib::PicoResult res{};

    auto start_time = std::chrono::high_resolution_clock::now();
    res = picol.RunCascade(nrows, ncols, pixels, ldim);
    res = picol.updateMemory(res);

    if(clustering) {
        res = pidroidlib::Pico::clusterDetections(res, 0.2);
    }

    if(res.ndetections > 0 && prominentFaceOnly) {
        res = pidroidlib::Pico::getProminentFace(res);
    }

    pidroidlib::CascadeParams params = picol.getCascadeParams();

    std::vector<pidroidlib::PuplocDetection> eyes = std::vector<pidroidlib::PuplocDetection>();
    pidroidlib::FlpDetection landmarks = {};

    for(int i=0; i<res.ndetections; ++i) {

        eyes.clear();
        eyeArray.clear();
        landmarkArray.clear();
        landmarks.detections.clear();

        pidroidlib::PicoDetection det = res.detections[i];

        if(det.threshold>=params.minThreshold)  {
            int centerX = det.col;
            int centerY = det.row;
            int radius = det.scale / 2;

            if (radius > 50 && pupilDetectionEnable) {
                pidroidlib::PuplocDetection left = {
                        .row =      centerY - int(0.075*radius),
                        .col =     centerX- int(0.175*radius),
                        .scale =    int(det.scale * 0.35f),
                        .perturbs = perturbs
                };
                left = puploc1.runDetector(left, nrows, ncols, pixels, ldim, params.angle, false);

                pidroidlib::PuplocDetection right = {
                        .row =      centerY - int(0.075*radius),
                        .col =     centerX + int(0.175*radius),
                        .scale =    int(det.scale * 0.35f),
                        .perturbs = perturbs
                };
                right = puploc1.runDetector(right, nrows, ncols, pixels, ldim, params.angle, false);


                eyes.push_back(left);
                right.scale = left.scale;
                eyes.push_back(right);

                if(landmarkDetectionEnable) {
                    landmarks = flploc.getLandmarkPoints(left, right, nrows, ncols, pixels, ldim, perturbs);
                }
            }

            for(auto &&eye: eyes) {
                jobject faceEye = env->NewObject(jEye.classRef, jEye.init);
                jobject centerEyes = env->GetObjectField(faceEye, jEye.center);

                env->SetIntField(centerEyes, jPoint.x, eye.col);
                env->SetIntField(centerEyes, jPoint.y, eye.row);

                env->SetObjectField(faceEye, jEye.center, centerEyes);
                env->SetIntField(faceEye, jEye.radius, int(eye.scale * 0.2f));

                eyeArray.push_back(faceEye);
            }

            for(auto &&landmark: landmarks.detections) {
                jobject faceLandmark = env->NewObject(jLandmark.classRef, jLandmark.init);
                jobject centerLandmark = env->GetObjectField(faceLandmark, jLandmark.center);

                env->SetIntField(centerLandmark, jPoint.x, landmark.col);
                env->SetIntField(centerLandmark, jPoint.y, landmark.row);

                env->SetObjectField(faceLandmark, jLandmark.center, centerLandmark);
                env->SetIntField(faceLandmark, jLandmark.radius, int(landmark.scale * 0.15f));

                landmarkArray.push_back(faceLandmark);
            }



            int faceWidth = radius * 2;
            int faceHeight = faceWidth; // 1.58 is Habitual Face Width-Height Ratio
            int x = centerX - (faceWidth / 2);
            int y = centerY - (faceHeight / 2);

            jobject face = env->NewObject(jFace.classRef, jFace.init);
            jobject topLeft = env->GetObjectField(face, jFace.topLeft);

            env->SetIntField(topLeft, jPoint.x, x);
            env->SetIntField(topLeft, jPoint.y, y);
            env->SetObjectField(face, jFace.topLeft, topLeft);

            env->SetIntField(face, jFace.width, faceWidth);
            env->SetIntField(face, jFace.height, faceHeight);

            env->SetObjectField(face, jFace.eyes, vectorJObjectToArrayList(env, eyeArray));
            env->SetObjectField(face, jFace.landmarks, vectorJObjectToArrayList(env, landmarkArray));

            faceArray.push_back(face);
        }
    }

    env->SetObjectField(detection_info, jFaceDetectionResult.faces, vectorJObjectToArrayList(env, faceArray));
    env->SetBooleanField(detection_info, jFaceDetectionResult.detected, faceArray.size() > 0);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time).count();
    std::string logText = "Processing time: "+std::to_string(duration);

    __android_log_write(ANDROID_LOG_VERBOSE, "Detection", logText.c_str());

    faceArray.clear();
    eyeArray.clear();
    landmarkArray.clear();
}
