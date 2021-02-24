package com.suaro.realtimesample.capturer.interactor

import android.content.Context
import android.graphics.Bitmap
import com.suaro.realtimesample.capturer.ImageProcessor

class ImageProcessorInteractor(private var context: Context, private var listener: ImageProcessor.Interactor.FinishListener) : ImageProcessor.Interactor {

    private var faceDetectorFrameProcessor: FaceDetectorFrameProcessor

    private var faceDetectorProcessingThread: Thread

    init {
        faceDetectorFrameProcessor = FaceDetectorFrameProcessor(listener)
        faceDetectorProcessingThread = Thread(faceDetectorFrameProcessor)

        faceDetectorFrameProcessor.setProcessingThread(faceDetectorProcessingThread)
        faceDetectorFrameProcessor.setActive(true)

        faceDetectorProcessingThread.start()
    }

    override fun detectFace(intArray: IntArray, width: Int, height: Int) {
        faceDetectorFrameProcessor.setNextFrame(intArray, width, height)
    }

    override fun detectFace(bitmap: Bitmap) {
        faceDetectorFrameProcessor.setNextFrame(bitmap)
    }
}