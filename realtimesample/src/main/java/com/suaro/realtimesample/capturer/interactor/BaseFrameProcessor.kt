package com.suaro.realtimesample.capturer.interactor

import android.graphics.Bitmap
import android.os.SystemClock
import com.suaro.realtimesample.capturer.ImageProcessor
import com.suaro.realtimesample.core.Utils
import java.util.concurrent.locks.ReentrantLock
import kotlin.concurrent.withLock

abstract class BaseFrameProcessor(protected var listener: ImageProcessor.Interactor.FinishListener) :
    Runnable {

    private val mStartTimeMillis = SystemClock.elapsedRealtime()

    // This lock guards all of the member variables below.
    protected val mLock = ReentrantLock()
    protected val condition = mLock.newCondition()

    protected var mActive = true
    private val processingThread: Thread? = null

    // These pending variables hold the state associated with the new frame awaiting processing.
    private var mPendingTimeMillis: Long = 0
    private var mPendingFrameId = 0
    protected var mPendingFrameData: IntArray? = null

    protected var previewWidth = 0
    protected var previewHeight = 0
    private var proccessingThread: Thread? = null


    fun setProcessingThread(proccessingThread: Thread?) {
        mLock.withLock {
            this.proccessingThread = proccessingThread
            condition.signalAll()
        }
    }

    /**
     * Marks the runnable as active/not active.  Signals any blocked threads to continue.
     */
    fun setActive(active: Boolean) {
        mLock.withLock {
            mActive = active
            condition.signalAll()
        }
    }

    /**
     * Sets the frame data received from the camera.  This adds the previous unused frame buffer
     * (if present) back to the camera, and keeps a pending reference to the frame data for
     * future use.
     */
    fun setNextFrame(
        data: IntArray?,
        previewWidth: Int,
        previewHeight: Int
    ) {
        mLock.withLock {
            this.previewHeight = previewHeight
            this.previewWidth = previewWidth

            if (mPendingFrameData != null) {
                mPendingFrameData = null
            }

            // Timestamp and frame ID are maintained here, which will give downstream code some
            // idea of the timing of frames received and when frames were dropped along the way.
            mPendingTimeMillis = SystemClock.elapsedRealtime() - mStartTimeMillis
            mPendingFrameId++
            mPendingFrameData = data

            // Notify the processor thread if it is waiting on the next frame (see below).
            condition.signalAll()
        }
    }

    /**
     * Sets the frame data received from the camera.  This adds the previous unused frame buffer
     * (if present) back to the camera, and keeps a pending reference to the frame data for
     * future use.
     */
    fun setNextFrame(data: Bitmap?
    ) {
        mLock.withLock {
            this.previewHeight = data!!.height
            this.previewWidth = data.width

            if (mPendingFrameData != null) {
                mPendingFrameData = null
            }

            // Timestamp and frame ID are maintained here, which will give downstream code some
            // idea of the timing of frames received and when frames were dropped along the way.
            mPendingTimeMillis = SystemClock.elapsedRealtime() - mStartTimeMillis
            mPendingFrameId++
            mPendingFrameData = Utils.getBytes(data)!!

            // Notify the processor thread if it is waiting on the next frame (see below).
            condition.signalAll()
        }
    }

    override fun run() {

    }

    init {

    }
}