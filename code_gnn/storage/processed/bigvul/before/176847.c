static jboolean disableNative(JNIEnv* env, jobject obj) {
    ALOGV("%s:",__FUNCTION__);

    jboolean result = JNI_FALSE;
 if (!sBluetoothInterface) return result;

 int ret = sBluetoothInterface->disable();
  
    result = (ret == BT_STATUS_FAIL) ? JNI_FALSE : JNI_TRUE;
 return result;
}
