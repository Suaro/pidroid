package com.suaro.imagesample

import com.suaro.pidroid.core.Point


class Circle {

    constructor(center: Point, radius: Int) {
        this.center = center
        this.radius = radius
    }

    var center: Point = Point(0,0)
    var radius: Int = 0
}