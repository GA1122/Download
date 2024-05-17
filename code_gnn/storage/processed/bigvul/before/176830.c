jint JNI_OnLoad(JavaVM *jvm, void *reserved)
{
 JNIEnv *e;
 int status;

    ALOGV("Bluetooth Adapter Service : loading JNI\n");

 if (jvm->GetEnv((void **)&e, JNI_VERSION_1_6)) {
        ALOGE("JNI version mismatch error");
 return JNI_ERR;
 }

 if ((status = android::register_com_android_bluetooth_btservice_AdapterService(e)) < 0) {
        ALOGE("jni adapter service registration failure, status: %d", status);
 return JNI_ERR;
 }

 if ((status = android::register_com_android_bluetooth_hfp(e)) < 0) {
        ALOGE("jni hfp registration failure, status: %d", status);
 return JNI_ERR;
 }

 if ((status = android::register_com_android_bluetooth_hfpclient(e)) < 0) {
        ALOGE("jni hfp client registration failure, status: %d", status);
 return JNI_ERR;
 }

 if ((status = android::register_com_android_bluetooth_a2dp(e)) < 0) {
        ALOGE("jni a2dp source registration failure: %d", status);
 return JNI_ERR;
 }

 if ((status = android::register_com_android_bluetooth_a2dp_sink(e)) < 0) {
        ALOGE("jni a2dp sink registration failure: %d", status);
 return JNI_ERR;
 }

 if ((status = android::register_com_android_bluetooth_avrcp(e)) < 0) {
        ALOGE("jni avrcp target registration failure: %d", status);
 return JNI_ERR;
 }

 if ((status = android::register_com_android_bluetooth_avrcp_controller(e)) < 0) {
        ALOGE("jni avrcp controller registration failure: %d", status);
 return JNI_ERR;
 }

 if ((status = android::register_com_android_bluetooth_hid(e)) < 0) {
        ALOGE("jni hid registration failure: %d", status);
 return JNI_ERR;
 }

 if ((status = android::register_com_android_bluetooth_hdp(e)) < 0) {
        ALOGE("jni hdp registration failure: %d", status);
 return JNI_ERR;
 }

 if ((status = android::register_com_android_bluetooth_pan(e)) < 0) {
        ALOGE("jni pan registration failure: %d", status);
 return JNI_ERR;
 }

 if ((status = android::register_com_android_bluetooth_gatt(e)) < 0) {
        ALOGE("jni gatt registration failure: %d", status);
 return JNI_ERR;
 }

 if ((status = android::register_com_android_bluetooth_sdp(e)) < 0) {
        ALOGE("jni sdp registration failure: %d", status);
 return JNI_ERR;
 }

 return JNI_VERSION_1_6;
}
