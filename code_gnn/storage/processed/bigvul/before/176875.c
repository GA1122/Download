static jboolean sspReplyNative(JNIEnv *env, jobject obj, jbyteArray address,
                               jint type, jboolean accept, jint passkey) {
    ALOGV("%s:",__FUNCTION__);

    jbyte *addr;
    jboolean result = JNI_FALSE;
 if (!sBluetoothInterface) return result;

    addr = env->GetByteArrayElements(address, NULL);
 if (addr == NULL) {
        jniThrowIOException(env, EINVAL);
 return result;
 }

 int ret = sBluetoothInterface->ssp_reply((bt_bdaddr_t *)addr,
 (bt_ssp_variant_t) type, accept, passkey);
    env->ReleaseByteArrayElements(address, addr, 0);
    result = (ret == BT_STATUS_SUCCESS) ? JNI_TRUE : JNI_FALSE;

 return result;
}
