package com.suaro.realtimesample.capturer.view

import android.content.Context
import android.graphics.Bitmap
import android.graphics.Rect
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import com.suaro.pidroid.core.FaceDetectionResult
import com.suaro.realtimesample.MainActivity
import com.suaro.realtimesample.R
import com.suaro.realtimesample.capturer.Capturer
import com.suaro.realtimesample.capturer.ImageProcessor
import com.suaro.realtimesample.capturer.entity.Circle
import com.suaro.realtimesample.capturer.presenter.ImageProcessorPresenter
import com.suaro.realtimesample.core.CameraPreviewListener


class CapturerFragment : Fragment(), ImageProcessor.View, CameraPreviewListener {
    private lateinit var capturerView: Capturer.View
    private lateinit var presenter: ImageProcessorPresenter
    private var canvasView: CanvasView? = null
    private var first: Boolean = true
    private var processing: Boolean = false
    private var mLock: Any = Object()

    override fun onAttach(context: Context) {
        super.onAttach(context)
        capturerView = if (context is Capturer.View) {
            context
        } else {
            throw ClassCastException(
                context.toString()
                    .toString() + " must implement ObInstructions.View"
            )
        }
        //TODO: Fix this...
        (activity as MainActivity).addCameraPreviewListener(this)
    }

    override fun onCreate(savedInstance: Bundle?) {
        super.onCreate(savedInstance)
    }

    override fun onCreateView(
        layoutInflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val v: View = layoutInflater.inflate(R.layout.capturer_fragment, container, false)
            ?: return null
        setRetainInstance(true)
        presenter = ImageProcessorPresenter(activity!!, this)
        canvasView = v.findViewById(R.id.canvas_view)
        return v
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {

    }

    override fun onResume() {
        super.onResume()
    }


    companion object {
        private val TAG = CapturerFragment::class.java.simpleName
        fun newInstance(): CapturerFragment {
            val f = CapturerFragment()
            //val args = Bundle()
            //args.putParcelable("instructions_item", item)
            //f.setArguments(args)
            return f
        }
    }

    override fun onFaceDetectorFinish(info: FaceDetectionResult) {
        this.activity?.runOnUiThread {
            synchronized(mLock) {
                processing = false;
                canvasView?.faces?.clear()
                canvasView?.eyes?.clear()
                canvasView?.landmarks?.clear()

                if (info.detected) {
                    //TODO: Check draw aspect ratio.
                    for (face in info.faces) {
                        var rect: Rect = Rect(
                            face.topLeft.x,
                            face.topLeft.y,
                            face.topLeft.x + face.width,
                            face.topLeft.y + face.height
                        )
                        canvasView?.faces?.add(rect)
                        for (eye in face.eyes) {
                            canvasView?.eyes?.add(Circle(eye.center, eye.radius))
                        }

                        for (landmark in face.landmarks) {
                            canvasView?.landmarks?.add(Circle(landmark.center, landmark.radius))
                        }
                    }

                }

                canvasView?.redraw()
            }
        }
    }

    override fun onPreviewFrame(bitmap: Bitmap) {
        if (first) {
            canvasView?.setAspectRatio(bitmap.width, bitmap.height)
            first = false
        }

        //if (processing)
        //    return

        processing = true
        this.presenter.detectFace(bitmap)
    }
}