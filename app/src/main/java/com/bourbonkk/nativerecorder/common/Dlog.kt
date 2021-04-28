package com.bourbonkk.nativerecorder.common

import android.util.Log

class Dlog {

    companion object {
        const val TAG = "RECORDER_NATIVE_TEST"
        val DEBUG = true

        /** Log Level Error  */
        @JvmStatic
        fun e(message: String) {
            if (DEBUG) Log.e(TAG, buildLogMsg(message))
        }

        /** Log Level Error  */
        @JvmStatic
        fun e(tag: String, message: String) {
            if (DEBUG) Log.e(tag, buildLogMsg(message))
        }

        /** Log Level Warning  */
        @JvmStatic
        fun w(message: String) {
            if (DEBUG) Log.w(TAG, buildLogMsg(message))
        }

        /** Log Level Warning  */
        @JvmStatic
        fun w(tag: String, message: String) {
            if (DEBUG) Log.w(tag, buildLogMsg(message))
        }

        /** Log Level Information  */
        @JvmStatic
        fun i(message: String) {
            if (DEBUG) Log.i(TAG, buildLogMsg(message))
        }

        /** Log Level Information  */
        @JvmStatic
        fun i(tag: String, message: String) {
            if (DEBUG) Log.i(tag, buildLogMsg(message))
        }

        /** Log Level Debug  */
        @JvmStatic
        fun d(message: String) {
            if (DEBUG) Log.d(TAG, buildLogMsg(message))
        }

        /** Log Level Debug  */
        @JvmStatic
        fun d(tag: String, message: String) {
            if (DEBUG) Log.d(tag, buildLogMsg(message))
        }

        /** Log Level Verbose  */
        @JvmStatic
        fun v(message: String) {
            if (DEBUG) Log.v(TAG, buildLogMsg(message))
        }

        /** Log Level Verbose  */
        @JvmStatic
        fun v(tag: String, message: String) {
            if (DEBUG) Log.v(tag, buildLogMsg(message))
        }

        @JvmStatic
        fun buildLogMsg(message: String): String {

            val ste = Thread.currentThread().stackTrace[4]

            val sb = StringBuilder()

            sb.append("[")
            sb.append(ste.fileName!!.replace(".java", ""))
            sb.append("::")
            sb.append(ste.methodName!!)
            sb.append("]")
            sb.append(message)

            return sb.toString()

        }
    }
}