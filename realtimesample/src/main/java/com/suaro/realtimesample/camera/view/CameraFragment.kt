package com.suaro.realtimesample.camera.view

import android.Manifest
import android.content.Context
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import androidx.fragment.app.Fragment
import com.suaro.realtimesample.R
import com.suaro.realtimesample.camera.Camera

class CameraFragment : Fragment() {

    private val CAMERA_PERMISSION: Int = 1001
    private lateinit var cameraView: Camera.View
    private lateinit var camera2: CameraManager
    private lateinit var v: View

    override fun onAttach(context: Context) {
        super.onAttach(context)
        cameraView = if (context is Camera.View) {
            context
        } else {
            throw ClassCastException(
                context.toString() + " must implement Camera.View"
            )
        }
    }

    override fun onCreate(savedInstance: Bundle?) {
        super.onCreate(savedInstance)

    }

    override fun onCreateView(
        layoutInflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        v = layoutInflater.inflate(R.layout.camera_fragment, container, false)
            ?: return null
        setRetainInstance(true)

        if (ContextCompat.checkSelfPermission(
                activity!!,
                Manifest.permission.CAMERA
            ) == PackageManager.PERMISSION_GRANTED
        )

            camera2 = CameraManager(activity!!, v.findViewById(R.id.camera_view), cameraView)
        else {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                ActivityCompat.requestPermissions(activity!!, arrayOf(Manifest.permission.CAMERA, Manifest.permission.WRITE_EXTERNAL_STORAGE), CAMERA_PERMISSION)
            }
            else {
                camera2 = CameraManager(activity!!, v.findViewById(R.id.camera_view), cameraView)
            }

        }

        return v
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {

    }

    override fun onResume() {
        super.onResume()
        if (::camera2.isInitialized) {
            camera2.start()
        }

    }

    override fun onPause() {
        super.onPause()
        if (::camera2.isInitialized) {
            camera2.stop()
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        when (requestCode) {
            CAMERA_PERMISSION -> {
                // If request is cancelled, the result arrays are empty.
                if ((grantResults.isNotEmpty() &&
                            grantResults[0] == PackageManager.PERMISSION_GRANTED)) {
                    camera2 = CameraManager(activity!!, v.findViewById(R.id.camera_view), cameraView)
                    if (::camera2.isInitialized) {
                        camera2.start()
                    }
                } else {
                    Toast.makeText(this.context, "You must accept camera permissions to continue", Toast.LENGTH_LONG).show()
                }
                return
            }

            else -> {
                // Ignore all other requests.
            }
        }
    }

    companion object {
        private val TAG = CameraFragment::class.java.simpleName
        fun newInstance(): CameraFragment {
            val f = CameraFragment()
            //val args = Bundle()
            //args.putParcelable("instructions_item", item)
            //f.setArguments(args)
            return f
        }
    }

}