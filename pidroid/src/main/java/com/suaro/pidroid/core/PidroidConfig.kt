package com.suaro.pidroid.core

class PidroidConfig {
    var minsize: Int = 150
    var maxsize: Int = 1000

    var angle: Float = 0f

    var scalefactor: Float = 1.1f
    var stridefactor: Float = 0.1f

    var qthreshold: Float = 3.0f

    var perturbs: Int = 10;

    var clustering: Boolean = true

    var pupilDetectionEnable: Boolean = true
    var landmarkDetectionEnable: Boolean = true

    var prominentFaceOnly: Boolean = true
}