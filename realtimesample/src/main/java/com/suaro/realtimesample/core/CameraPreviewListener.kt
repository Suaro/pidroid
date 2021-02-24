package com.suaro.realtimesample.core

import android.graphics.Bitmap

interface CameraPreviewListener {
    fun onPreviewFrame(bitmap: Bitmap);
}