package com.suaro.pidroid.core;

import org.jetbrains.annotations.NotNull;

public final class Eye {
    private Point center = new Point(0, 0);
    private int radius;

    @NotNull
    public final Point getCenter() {
        return this.center;
    }

    public final void setCenter(@NotNull Point point) {
        this.center = point;
    }

    public final int getRadius() {
        return this.radius;
    }

    public final void setRadius(int rad) {
        this.radius = rad;
    }
}