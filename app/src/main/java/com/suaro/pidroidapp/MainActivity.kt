package com.suaro.pidroidapp

import android.content.res.AssetManager
import android.graphics.Bitmap
import android.os.Bundle
import android.util.Log
import android.widget.FrameLayout
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.FragmentTransaction
import com.suaro.pidroid.Pidroid
import com.suaro.pidroid.core.PidroidConfig
import com.suaro.pidroidapp.camera.Camera
import com.suaro.pidroidapp.camera.view.CameraFragment
import com.suaro.pidroidapp.capturer.Capturer
import com.suaro.pidroidapp.capturer.view.CapturerFragment
import com.suaro.pidroidapp.core.CameraPreviewListener
import java.util.*


class MainActivity : AppCompatActivity(), Camera.View, Capturer.View{

    private lateinit var capturerLayout: FrameLayout
    private lateinit var cameraLayout: FrameLayout

    private lateinit var capturerFragment: CapturerFragment
    private lateinit var cameraFragment: CameraFragment

    private var cameraPreviewListenerList: ArrayList<CameraPreviewListener>? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        cameraLayout = findViewById(R.id.camera_fragment)
        capturerLayout = findViewById(R.id.capturer_fragment)

        setupNative()
        loadCameraFragment()
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

    fun loadCameraFragment() {
        cameraFragment = CameraFragment.newInstance();
        val transaction: FragmentTransaction = supportFragmentManager.beginTransaction()
        transaction.replace(R.id.camera_fragment, cameraFragment)
        transaction.commit()
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
