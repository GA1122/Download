static jboolean setAdapterPropertyNative(JNIEnv *env, jobject obj, jint type, jbyteArray value) {
    ALOGV("%s:",__FUNCTION__);

    jbyte *val;
    jboolean result = JNI_FALSE;
 if (!sBluetoothInterface) return result;

    val = env->GetByteArrayElements(value, NULL);
 bt_property_t prop;
    prop.type = (bt_property_type_t) type;
    prop.len = env->GetArrayLength(value);
    prop.val = val;

 int ret = sBluetoothInterface->set_adapter_property(&prop);
    env->ReleaseByteArrayElements(value, val, 0);
    result = (ret == BT_STATUS_SUCCESS) ? JNI_TRUE : JNI_FALSE;

 return result;
}
