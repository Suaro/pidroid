package com.suaro.pidroidapp.core

import android.graphics.Bitmap

interface CameraPreviewListener {
    fun onPreviewFrame(bitmap: Bitmap);
}