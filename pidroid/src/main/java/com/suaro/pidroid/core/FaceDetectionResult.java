package com.suaro.pidroid.core;

import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;

public final class FaceDetectionResult {
    @NotNull
    private ArrayList<Face> faces = new ArrayList<Face>();
    private boolean detected;

    @NotNull
    public final ArrayList<Face> getFaces() {
        return this.faces;
    }

    public final void setFaces(@NotNull ArrayList<Face> faces) {
        this.faces = faces;
    }

    public final boolean getDetected() {
        return this.detected;
    }

    public final void setDetected(boolean detected) {
        this.detected = detected;
    }
}