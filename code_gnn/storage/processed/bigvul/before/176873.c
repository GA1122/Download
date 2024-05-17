static jboolean setDevicePropertyNative(JNIEnv *env, jobject obj, jbyteArray address,
                                        jint type, jbyteArray value) {
    ALOGV("%s:",__FUNCTION__);

    jbyte *val, *addr;
    jboolean result = JNI_FALSE;
 if (!sBluetoothInterface) return result;

    val = env->GetByteArrayElements(value, NULL);
 if (val == NULL) {
        jniThrowIOException(env, EINVAL);
 return result;
 }

    addr = env->GetByteArrayElements(address, NULL);
 if (addr == NULL) {
        env->ReleaseByteArrayElements(value, val, 0);
        jniThrowIOException(env, EINVAL);
 return result;
 }


 bt_property_t prop;
    prop.type = (bt_property_type_t) type;
    prop.len = env->GetArrayLength(value);
    prop.val = val;

 int ret = sBluetoothInterface->set_remote_device_property((bt_bdaddr_t *)addr, &prop);
    env->ReleaseByteArrayElements(value, val, 0);
    env->ReleaseByteArrayElements(address, addr, 0);

    result = (ret == BT_STATUS_SUCCESS) ? JNI_TRUE : JNI_FALSE;

 return result;
}
