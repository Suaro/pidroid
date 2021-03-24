package com.suaro.pidroid.core;

public final class Point {
    private int x;
    private int y;

    public final int getX() {
        return this.x;
    }

    public final void setX(int var1) {
        this.x = var1;
    }

    public final int getY() {
        return this.y;
    }

    public final void setY(int var1) {
        this.y = var1;
    }

    public Point(int x, int y) {
        this.x = x;
        this.y = y;
    }
}