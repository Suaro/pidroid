package com.suaro.pidroid;

import android.content.Context;
import android.content.res.AssetManager;
import android.content.res.Resources;

import com.suaro.pidroid.core.FaceDetectionResult;
import com.suaro.pidroid.core.NativeMethods;
import com.suaro.pidroid.core.PidroidConfig;

import org.jetbrains.annotations.NotNull;


public final class Pidroid {
    private static AssetManager mgr;
    private static PidroidConfig cascadeConfig;



    public static void setup(@NotNull Context context, @NotNull PidroidConfig config) {
        mgr = context.getResources().getAssets();
        cascadeConfig = config;
        NativeMethods.setup(cascadeConfig, mgr);
    }

    public static void detectFace(@NotNull int[] byteArray, int width, int height, @NotNull FaceDetectionResult dInfo) {
        NativeMethods.detectFace(byteArray, width, height, dInfo);
    }

    private Pidroid() {
    }

}