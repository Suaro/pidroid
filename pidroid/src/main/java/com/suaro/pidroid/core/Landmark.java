package com.suaro.pidroid.core;

import org.jetbrains.annotations.NotNull;

public final class Landmark {
    @NotNull
    private Point center = new Point(0, 0);
    private int radius;

    @NotNull
    public final Point getCenter() {
        return this.center;
    }

    public final void setCenter(@NotNull Point center) {
        this.center = center;
    }

    public final int getRadius() {
        return this.radius;
    }

    public final void setRadius(int radius) {
        this.radius = radius;
    }
}