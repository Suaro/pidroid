package com.suaro.pidroid.core;

public final class PidroidConfig {
    private int minsize = 150;
    private int maxsize = 1000;
    private float scalefactor = 1.1F;
    private float stridefactorWidth = 0.2F;
    private float stridefactorHeight = 0.12F;
    private float qthreshold = 3.0F;
    private int perturbs = 10;
    private boolean clustering = true;
    private boolean pupilDetectionEnable = true;
    private boolean landmarkDetectionEnable = true;
    private boolean prominentFaceOnly;

    public int getMinsize() {
        return minsize;
    }

    public void setMinsize(int minsize) {
        this.minsize = minsize;
    }

    public int getMaxsize() {
        return maxsize;
    }

    public void setMaxsize(int maxsize) {
        this.maxsize = maxsize;
    }

    public float getScalefactor() {
        return scalefactor;
    }

    public void setScalefactor(float scalefactor) {
        this.scalefactor = scalefactor;
    }

    public float getStridefactorWidth() {
        return stridefactorWidth;
    }

    public void setStridefactorWidth(float stridefactorWidth) {
        this.stridefactorWidth = stridefactorWidth;
    }

    public float getStridefactorHeight() {
        return stridefactorHeight;
    }

    public void setStridefactorHeight(float stridefactorHeight) {
        this.stridefactorHeight = stridefactorHeight;
    }

    public float getQthreshold() {
        return qthreshold;
    }

    public void setQthreshold(float qthreshold) {
        this.qthreshold = qthreshold;
    }

    public int getPerturbs() {
        return perturbs;
    }

    public void setPerturbs(int perturbs) {
        this.perturbs = perturbs;
    }

    public boolean getClustering() {
        return clustering;
    }

    public void setClustering(boolean clustering) {
        this.clustering = clustering;
    }

    public boolean getPupilDetectionEnable() {
        return pupilDetectionEnable;
    }

    public void setPupilDetectionEnable(boolean pupilDetectionEnable) {
        this.pupilDetectionEnable = pupilDetectionEnable;
    }

    public boolean getLandmarkDetectionEnable() {
        return landmarkDetectionEnable;
    }

    public void setLandmarkDetectionEnable(boolean landmarkDetectionEnable) {
        this.landmarkDetectionEnable = landmarkDetectionEnable;
    }

    public boolean getProminentFaceOnly() {
        return prominentFaceOnly;
    }

    public void setProminentFaceOnly(boolean prominentFaceOnly) {
        this.prominentFaceOnly = prominentFaceOnly;
    }
}