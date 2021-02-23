package com.suaro.pidroid.core

import android.content.res.AssetManager


class NativeMethods {

    companion object {
        init {
            System.loadLibrary("pidroid")
        }

        external fun setup(config: PidroidConfig, mgr: AssetManager)
        external fun detectFace(rgbaBytes: IntArray, width: Int, height: Int, detectionInfo: FaceDetectionResult)
    }
}