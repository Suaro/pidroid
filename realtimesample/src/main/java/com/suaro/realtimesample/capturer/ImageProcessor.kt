package com.suaro.realtimesample.capturer

import android.graphics.Bitmap
import com.suaro.pidroid.core.FaceDetectionResult


interface ImageProcessor {
    interface Interactor {
        fun detectFace(intArray: IntArray, width: Int, height: Int)
        fun detectFace(bitmap: Bitmap)

        interface FinishListener {
            fun onFaceDetectorFinish(info: FaceDetectionResult)
        }
    }

    interface View {
        fun onFaceDetectorFinish(info: FaceDetectionResult)
    }

    interface Presenter {
        fun detectFace(intArray: IntArray, width: Int, height: Int)
        fun detectFace(bitmap: Bitmap)

    }
}