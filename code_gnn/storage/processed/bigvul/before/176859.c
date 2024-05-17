static jboolean getRemoteServicesNative(JNIEnv *env, jobject obj, jbyteArray address) {
    ALOGV("%s:",__FUNCTION__);

    jbyte *addr = NULL;
    jboolean result = JNI_FALSE;
 if (!sBluetoothInterface) return result;

    addr = env->GetByteArrayElements(address, NULL);
 if (addr == NULL) {
        jniThrowIOException(env, EINVAL);
 return result;
 }

 int ret = sBluetoothInterface->get_remote_services((bt_bdaddr_t *)addr);
    env->ReleaseByteArrayElements(address, addr, 0);
    result = (ret == BT_STATUS_SUCCESS) ? JNI_TRUE : JNI_FALSE;
 return result;
}
