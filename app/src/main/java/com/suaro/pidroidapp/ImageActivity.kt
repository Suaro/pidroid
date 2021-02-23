package com.suaro.pidroidapp

import android.content.res.AssetManager
import android.graphics.Bitmap
import android.graphics.drawable.BitmapDrawable
import android.os.Bundle
import android.os.Handler
import android.util.Log
import android.widget.FrameLayout
import android.widget.ImageView
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.FragmentTransaction
import com.suaro.pidroid.Pidroid
import com.suaro.pidroid.core.PidroidConfig
import com.suaro.pidroidapp.camera.Camera
import com.suaro.pidroidapp.capturer.Capturer
import com.suaro.pidroidapp.capturer.view.CapturerFragment
import com.suaro.pidroidapp.core.CameraPreviewListener
import java.util.*


class ImageActivity : AppCompatActivity(), Camera.View, Capturer.View{

    private lateinit var capturerLayout: FrameLayout
    private lateinit var imageTest: ImageView

    private lateinit var capturerFragment: CapturerFragment

    private var cameraPreviewListenerList: ArrayList<CameraPreviewListener>? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(com.suaro.pidroidapp.R.layout.activity_image)

        capturerLayout = findViewById(R.id.capturer_fragment)
        imageTest = findViewById(R.id.photo)
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
        onCameraStart()
        handler.postDelayed({
            val drawable: BitmapDrawable = imageTest.getDrawable() as BitmapDrawable
            val bitmap: Bitmap = drawable.bitmap
            onPreviewFrame(bitmap)
        }, 1000)

    }

    override fun onDestroy() {
        super.onDestroy()
    }

    override fun onCameraStart() {
        loadCapturerFragment()
    }

    override fun onPreviewFrame(bitmap: Bitmap) {
        cameraPreviewListenerList!!.forEach {
            it.onPreviewFrame(bitmap)
        }
    }

    override fun onCapturerFinish() {
        Log.i("Capturer", "Finish")
    }


    fun loadCapturerFragment() {
        capturerFragment = CapturerFragment.newInstance();
        val transaction: FragmentTransaction = supportFragmentManager.beginTransaction()
        transaction.replace(R.id.capturer_fragment, capturerFragment)
        transaction.commit()
    }

    fun addCameraPreviewListener(listener: CameraPreviewListener) {
        if (cameraPreviewListenerList == null) {
            cameraPreviewListenerList = ArrayList<CameraPreviewListener>()
        }
        cameraPreviewListenerList!!.add(listener)
    }
}
