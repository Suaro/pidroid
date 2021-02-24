package com.suaro.realtimesample.camera

import android.graphics.Bitmap

interface Camera {
    interface View{
        fun onCameraStart()
        fun onPreviewFrame(bitmap: Bitmap)
    }

    interface Presenter {

    }
}