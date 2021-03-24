package com.suaro.pidroid.core;

import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;


public final class Face {

    @NotNull
    private Point topLeft = new Point(0, 0);
    private int width;
    private int height;

    @NotNull
    private ArrayList<Eye> eyes = new ArrayList<Eye>();

    @NotNull
    private ArrayList<Landmark> landmarks = new ArrayList<Landmark>();

    @NotNull
    public final Point getTopLeft() {
        return this.topLeft;
    }

    public final void setTopLeft(@NotNull Point topLeft) {
        this.topLeft = topLeft;
    }

    public final int getWidth() {
        return this.width;
    }

    public final void setWidth(int width) {
        this.width = width;
    }

    public final int getHeight() {
        return this.height;
    }

    public final void setHeight(int height) {
        this.height = height;
    }

    @NotNull
    public final ArrayList<Eye> getEyes() {
        return this.eyes;
    }

    public final void setEyes(@NotNull ArrayList<Eye> eyes) {
        this.eyes = eyes;
    }

    @NotNull
    public final ArrayList<Landmark> getLandmarks() {
        return this.landmarks;
    }

    public final void setLandmarks(@NotNull ArrayList<Landmark> landmarks) {
        this.landmarks = landmarks;
    }
}