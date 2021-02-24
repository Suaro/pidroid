package com.suaro.realtimesample.capturer.interactor

import android.util.Log
import com.suaro.pidroid.Pidroid
import com.suaro.pidroid.core.FaceDetectionResult
import com.suaro.realtimesample.capturer.ImageProcessor
import kotlin.concurrent.withLock

class FaceDetectorFrameProcessor(listener: ImageProcessor.Interactor.FinishListener) :
    BaseFrameProcessor(listener) {

    companion object {
        private val TAG = FaceDetectorFrameProcessor::class.java.simpleName
    }

    fun detect(byteArray: IntArray, width: Int, height: Int): FaceDetectionResult {
        var dInfo = FaceDetectionResult()
        Pidroid.detectFace(byteArray, width, height, dInfo)
        return dInfo
    }

    override fun run() {
        super.run()
        var outputFrame: IntArray

        while (true) {
            mLock.withLock {
                while (mActive && mPendingFrameData == null) {
                    try {
                        condition.await()
                    } catch (e: InterruptedException) {
                        return
                    }
                }
                if (!mActive) {
                    return
                }
                outputFrame = mPendingFrameData!!
                mPendingFrameData = null
            }
            try {
                val detectionInfo: FaceDetectionResult = this.detect(outputFrame, previewWidth, previewHeight)
                listener.onFaceDetectorFinish(detectionInfo)
            } catch (t: Throwable) {
                Log.e(TAG, t.message.toString())
            }
        }

    }
}

