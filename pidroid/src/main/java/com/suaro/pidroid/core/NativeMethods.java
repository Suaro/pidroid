package com.suaro.pidroid.core;

import android.content.res.AssetManager;

import org.jetbrains.annotations.NotNull;


public final class NativeMethods {
    static {
        System.loadLibrary("pidroid");
    }
    public static native void setup(@NotNull PidroidConfig var1, @NotNull AssetManager var2);

    public static native void detectFace(@NotNull int[] var1, int var2, int var3, @NotNull FaceDetectionResult var4);
}