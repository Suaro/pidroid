package com.suaro.pidroidapp.core

import android.graphics.Bitmap
import android.graphics.ImageFormat
import android.graphics.Rect
import android.graphics.YuvImage
import android.media.Image
import java.io.ByteArrayOutputStream
import java.nio.ByteBuffer


class Utils {

    companion object {

        fun getBytes(image: Bitmap): IntArray? {
            val pixels = IntArray(image.width * image.height * 4)
            image.getPixels(pixels, 0, image.width, 0, 0, image.width, image.height)
            return pixels
        }

        fun bitmapToBgrBytes(image: Bitmap): ByteArray? {

            // calculate how many bytes our image consists of
            val bytes = image.byteCount
            val buffer = ByteBuffer.allocate(bytes) // Create a new buffer
            image.copyPixelsToBuffer(buffer) // Move the byte data to the buffer
            val temp = buffer.array() // Get the underlying array containing the data.
            val pixels = ByteArray(temp.size / 4 * 3) // Allocate for 3 byte BGR

            // Copy pixels into place
            for (i in 0 until temp.size / 4) {
                pixels[i * 3] = temp[i * 4 + 3] // B
                pixels[i * 3 + 1] = temp[i * 4 + 2] // G
                pixels[i * 3 + 2] = temp[i * 4 + 1] // R

                // Alpha is discarded
            }
            return pixels
        }

        fun bitmapToGrayBytes(image: Bitmap): ByteArray? {
            // calculate how many bytes our image consists of
            val bytes = image.byteCount
            val buffer = ByteBuffer.allocate(bytes) // Create a new buffer
            image.copyPixelsToBuffer(buffer) // Move the byte data to the buffer
            val temp = buffer.array() // Get the underlying array containing the data.
            val pixels = ByteArray(temp.size / 4) //RGBA (4 channel) size is 4x greater than Gray (One channel) size

            var b: Double = 0.0
            var g: Double = 0.0
            var r: Double = 0.0

            var gray: Double = 0.0
            // Copy pixels into place
            for (i in 0 until temp.size / 4) {
                b =  temp[i * 4 + 2].toUByte().toDouble()
                g = temp[i * 4 + 1].toUByte().toDouble()
                r = temp[i * 4 + 0].toUByte().toDouble()

                gray = 0.2 * r + 0.7*g + 0.1*b

                pixels[i] = gray.toInt().toByte()
            }
            return pixels
        }

        fun bitmapToByteArray(bmp: Bitmap): ByteArray {
            val stream = ByteArrayOutputStream()
            bmp.compress(Bitmap.CompressFormat.PNG, 100, stream)
            val byteArray: ByteArray = stream.toByteArray()
            bmp.recycle()
            return byteArray
        }

        fun YUV_420_888toNV21(image: Image): ByteArray? {
            val nv21: ByteArray
            val yBuffer: ByteBuffer = image.getPlanes().get(0).getBuffer()
            val uBuffer: ByteBuffer = image.getPlanes().get(1).getBuffer()
            val vBuffer: ByteBuffer = image.getPlanes().get(2).getBuffer()
            val ySize: Int = yBuffer.remaining()
            val uSize: Int = uBuffer.remaining()
            val vSize: Int = vBuffer.remaining()
            nv21 = ByteArray(ySize + uSize + vSize)

            //U and V are swapped
            yBuffer.get(nv21, 0, ySize)
            vBuffer.get(nv21, ySize, vSize)
            uBuffer.get(nv21, ySize + vSize, uSize)
            return nv21
        }


        fun NV21toJPEG(nv21: ByteArray, width: Int, height: Int): ByteArray? {
            val out = ByteArrayOutputStream()
            val yuv = YuvImage(nv21, ImageFormat.NV21, width, height, null)
            yuv.compressToJpeg(Rect(0, 0, width, height), 100, out)
            return out.toByteArray()
        }
    }
}