static jboolean getAdapterPropertiesNative(JNIEnv *env, jobject obj) {
    ALOGV("%s:",__FUNCTION__);

    jboolean result = JNI_FALSE;
 if (!sBluetoothInterface) return result;

 int ret = sBluetoothInterface->get_adapter_properties();
    result = (ret == BT_STATUS_SUCCESS) ? JNI_TRUE : JNI_FALSE;

 return result;
}
