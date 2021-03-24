package com.suaro.imagesample

import android.graphics.Bitmap
import android.graphics.Rect
import android.graphics.drawable.BitmapDrawable
import android.os.Bundle
import android.os.Handler
import android.widget.ImageView
import androidx.appcompat.app.AppCompatActivity
import com.suaro.pidroid.Pidroid
import com.suaro.pidroid.core.FaceDetectionResult
import com.suaro.pidroid.core.PidroidConfig


class ImageActivity : AppCompatActivity() {

    private lateinit var imageTest: ImageView
    private lateinit var canvasView: CanvasView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_image)

        imageTest = findViewById(R.id.photo)
        canvasView = findViewById(R.id.canvas_view)
        setupNative()
    }

    private fun setupNative() {
        val pidroidConfig = PidroidConfig()
        Pidroid.setup(this, pidroidConfig)
    }

    override fun onPause() {
        super.onPause()
    }

    override fun onResume() {
        super.onResume()
        val handler: Handler = Handler();

        handler.postDelayed({
            performFaceDetection()
        }, 100)
    }

    override fun onDestroy() {
        super.onDestroy()
    }

    private fun performFaceDetection() {
        val drawable: BitmapDrawable = imageTest.getDrawable() as BitmapDrawable
        val bitmap: Bitmap = drawable.bitmap
        canvasView.setAspectRatio(bitmap.width, bitmap.height)
        val dinfo = callDetectFace(bitmap)
        drawResult(dinfo)
    }

    fun callDetectFace(bitmap: Bitmap): FaceDetectionResult {
        val pixels = IntArray(bitmap.width * bitmap.height * 4)
        bitmap.getPixels(pixels, 0, bitmap.width, 0, 0, bitmap.width, bitmap.height)
        val dInfo = FaceDetectionResult()
        Pidroid.detectFace(pixels, bitmap.width, bitmap.height, dInfo)
        return dInfo
    }

    fun drawResult(info: FaceDetectionResult) {
            canvasView.faces.clear()
            canvasView.eyes.clear()
            canvasView.landmarks.clear()

            if (info.detected) {
                for (face in info.faces) {
                    val rect = Rect(
                        face.topLeft.x,
                        face.topLeft.y,
                        face.topLeft.x + face.width,
                        face.topLeft.y + face.height
                    )
                    canvasView.faces.add(rect)
                    for (eye in face.eyes) {
                        canvasView.eyes.add(Circle(eye.center, eye.radius))
                    }

                    for (landmark in face.landmarks) {
                        canvasView.landmarks.add(Circle(landmark.center, landmark.radius))
                    }
                }

            }

            canvasView.redraw()

    }

}
