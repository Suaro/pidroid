package com.suaro.realtimesample.capturer.view

import android.content.Context
import android.graphics.*
import android.util.AttributeSet
import android.view.View
import com.suaro.realtimesample.capturer.entity.Circle
import com.suaro.pidroid.core.Point

class CanvasView @JvmOverloads constructor(context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0): View(context, attrs, defStyleAttr) {

    private val RECT_BORDER_RADIUS = 25f;
    private val RECT_BORDER_WIDTH = 10f;

    var faces: ArrayList<Rect> = ArrayList();
    var eyes: ArrayList<Circle> = ArrayList();
    var landmarks: ArrayList<Circle> = ArrayList();

    var paint: Paint = Paint()

    private var mRatioWidth: Double = 0.0
    private var mRatioHeight: Double = 0.0

    private var lastRatioWidth: Double = 0.0
    private var lastRatioHeight: Double = 0.0

    private var currentRatio: Double = 0.0
    private var currentWidth: Double = 0.0
    private var currentHeight: Double = 0.0

    private var widthScaleFactor: Double = 1.0;
    private var heightScaleFactor: Double = 1.0;

    fun drawRect(rect: Rect, canvas: Canvas?, color: Int = Color.GREEN) {;

        paint.setStyle(Paint.Style.STROKE)
        paint.setColor(color)
        paint.setStrokeWidth(RECT_BORDER_WIDTH);

        canvas?.drawRoundRect(rect.left.toFloat(), rect.top.toFloat(), rect.right.toFloat(), rect.bottom.toFloat(), RECT_BORDER_RADIUS, RECT_BORDER_RADIUS, paint);
    }

    fun drawCircle(circle: Circle, canvas: Canvas?, color: Int = Color.GREEN) {;

        paint.setStyle(Paint.Style.FILL)
        paint.setColor(color)

        canvas?.drawCircle(
                circle.center.x.toFloat(),
                circle.center.y.toFloat(), circle.radius.toFloat(), paint)
    }

    // Called when the view should render its content.
    override fun onDraw(canvas: Canvas?) {
        super.onDraw(canvas)

        faces.forEach {
            drawRect(getScaleRect(it), canvas, Color.GREEN);
        }

        eyes.forEach {
            drawCircle(scaleCircle(it), canvas, Color.RED);
        }

        landmarks.forEach {
            drawCircle(scaleCircle(it), canvas, Color.BLUE);
        }
    }

    private fun scaleCircle(it: Circle): Circle {
        val x = translateX(it.center.x.toInt())
        val y = translateY(it.center.y.toInt())
        val radius = scaleY(it.radius)

        return Circle(Point(x.toInt(),y.toInt()), radius.toInt())
    }

    private fun getScaleRect(rect: Rect): Rect {
        val xRect = rect.left;
        val yRect = rect.top;
        val x: Float = translateX(xRect + rect.width() / 2)
        val y: Float = translateY(yRect + rect.height() / 2)

        // Draws a bounding box around the face.
        val xOffset: Float = scaleX((rect.width() / 2.0f).toInt())
        val left = x - xOffset
        val right = x + xOffset
        val width = right - left
        val height = width
        val yOffset = height / 2
        val top = y - yOffset
        val bottom = y + yOffset
        return Rect(left.toInt(), top.toInt(), right.toInt(), bottom.toInt())
    }

    private fun translateX(x: Int): Float {
        return scaleX(x)
    }

    private fun scaleX(x: Int): Float {
        return (x*widthScaleFactor).toFloat();
    }

    private fun translateY(y: Int): Float {
        return scaleY(y)
    }

    private fun scaleY(y: Int): Float {
        return (y*heightScaleFactor).toFloat();
    }

    fun redraw() {
        this.invalidate()
    }

    fun setAspectRatio(width: Int, height: Int) {
        if (width < 0 || height < 0) {
            throw IllegalArgumentException("Size cannot be negative.")
        }

        mRatioWidth = width.toDouble()
        mRatioHeight = height.toDouble()

        if (this.measuredWidth < this.measuredHeight * mRatioWidth / mRatioHeight) {
            currentWidth = this.measuredWidth.toDouble()
            currentRatio = mRatioHeight / mRatioWidth
            currentHeight = this.measuredWidth * currentRatio
        } else {
            currentHeight = this.measuredHeight.toDouble()
            currentRatio = mRatioWidth / mRatioHeight
            currentWidth = this.measuredHeight * currentRatio
        }


        requestLayout()
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec)
        val width = MeasureSpec.getSize(widthMeasureSpec)
        val height = MeasureSpec.getSize(heightMeasureSpec)
        if (0.0 == this.currentWidth || 0.0 == this.currentHeight) {
            currentWidth = width.toDouble()
            currentHeight = height.toDouble()
        }

        widthScaleFactor = currentWidth / mRatioWidth;
        heightScaleFactor = currentHeight / mRatioHeight;

        setMeasuredDimension(currentWidth.toInt(), currentHeight.toInt())
        this.invalidate()
    }
}