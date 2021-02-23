package com.suaro.pidroid.core

class Face {
    var topLeft: Point = Point(0,0)
    var width: Int = 0
    var height: Int = 0
    var eyes: ArrayList<Eye> = ArrayList()
    var landmarks: ArrayList<Landmark> = ArrayList()
}