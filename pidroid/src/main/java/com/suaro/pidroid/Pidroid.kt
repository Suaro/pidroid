package com.suaro.pidroid

import android.content.Context
import android.content.res.AssetManager
import com.suaro.pidroid.core.FaceDetectionResult
import com.suaro.pidroid.core.PidroidConfig
import com.suaro.pidroid.core.NativeMethods

object Pidroid {

    private lateinit var mgr: AssetManager
    private lateinit var cascadeConfig: PidroidConfig


    fun setup(context: Context, config: PidroidConfig) {
        this.mgr = context.resources.assets
        this.cascadeConfig = config

        NativeMethods.setup(cascadeConfig, mgr)
    }

    fun detectFace(byteArray: IntArray, width: Int, height: Int, dInfo: FaceDetectionResult) {
        NativeMethods.detectFace(byteArray, width, height, dInfo);
    }


}