static void alarmFiredNative(JNIEnv *env, jobject obj) {
 if (sAlarmCallback) {
        sAlarmCallback(sAlarmCallbackData);
 } else {
        ALOGE("%s() - Alarm fired with callback not set!", __FUNCTION__);
 }
}
