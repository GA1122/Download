static jboolean removeBondNative(JNIEnv* env, jobject obj, jbyteArray address) {
    ALOGV("%s:",__FUNCTION__);

    jbyte *addr;
    jboolean result;
 if (!sBluetoothInterface) return JNI_FALSE;

    addr = env->GetByteArrayElements(address, NULL);
 if (addr == NULL) {
        jniThrowIOException(env, EINVAL);
 return JNI_FALSE;
 }

 int ret = sBluetoothInterface->remove_bond((bt_bdaddr_t *)addr);
    env->ReleaseByteArrayElements(address, addr, 0);
    result = (ret == BT_STATUS_SUCCESS) ? JNI_TRUE : JNI_FALSE;

 return result;
}
