package com.suaro.pidroidapp.camera

import android.graphics.Bitmap

interface Camera {
    interface View{
        fun onCameraStart()
        fun onPreviewFrame(bitmap: Bitmap)
    }

    interface Presenter {

    }
}