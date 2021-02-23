package com.suaro.pidroidapp.capturer.presenter

import android.content.Context
import android.graphics.Bitmap
import com.suaro.pidroid.core.FaceDetectionResult
import com.suaro.pidroidapp.capturer.ImageProcessor
import com.suaro.pidroidapp.capturer.interactor.ImageProcessorInteractor

class ImageProcessorPresenter(private var context: Context, private val view: ImageProcessor.View): ImageProcessor.Presenter, ImageProcessor.Interactor.FinishListener {

    private val interactor: ImageProcessorInteractor

    init {
        interactor = ImageProcessorInteractor(context, this)
    }

    override fun detectFace(intArray: IntArray, width: Int, height: Int) {
        interactor.detectFace(intArray, width, height)
    }

    override fun detectFace(bitmap: Bitmap) {
        interactor.detectFace(bitmap)
    }

    override fun onFaceDetectorFinish(info: FaceDetectionResult) {
        view.onFaceDetectorFinish(info)
    }
}