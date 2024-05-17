static jboolean getAdapterPropertyNative(JNIEnv *env, jobject obj, jint type) {
    ALOGV("%s:",__FUNCTION__);

    jboolean result = JNI_FALSE;
 if (!sBluetoothInterface) return result;

 int ret = sBluetoothInterface->get_adapter_property((bt_property_type_t) type);
    result = (ret == BT_STATUS_SUCCESS) ? JNI_TRUE : JNI_FALSE;

 return result;
}
