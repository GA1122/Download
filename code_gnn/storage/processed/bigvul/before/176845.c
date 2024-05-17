static jboolean createBondNative(JNIEnv* env, jobject obj, jbyteArray address, jint transport) {
    ALOGV("%s:",__FUNCTION__);

    jbyte *addr;
    jboolean result = JNI_FALSE;

 if (!sBluetoothInterface) return result;

    addr = env->GetByteArrayElements(address, NULL);
 if (addr == NULL) {
        jniThrowIOException(env, EINVAL);
 return result;
 }

 int ret = sBluetoothInterface->create_bond((bt_bdaddr_t *)addr, transport);
    env->ReleaseByteArrayElements(address, addr, 0);
    result = (ret == BT_STATUS_SUCCESS) ? JNI_TRUE : JNI_FALSE;

 return result;
}
