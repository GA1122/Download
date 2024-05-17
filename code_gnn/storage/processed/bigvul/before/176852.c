static jboolean factoryResetNative(JNIEnv *env, jobject obj) {
    ALOGV("%s:", __FUNCTION__);
 if (!sBluetoothInterface) return JNI_FALSE;
 int ret = sBluetoothInterface->config_clear();
 return (ret == BT_STATUS_SUCCESS) ? JNI_TRUE : JNI_FALSE;
}
