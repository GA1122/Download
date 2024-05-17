static jboolean pinReplyNative(JNIEnv *env, jobject obj, jbyteArray address, jboolean accept,
                               jint len, jbyteArray pinArray) {
    ALOGV("%s:",__FUNCTION__);

    jbyte *addr, *pinPtr = NULL;
    jboolean result = JNI_FALSE;
 if (!sBluetoothInterface) return result;

    addr = env->GetByteArrayElements(address, NULL);
 if (addr == NULL) {
        jniThrowIOException(env, EINVAL);
 return result;
 }

 if (accept) {
        pinPtr = env->GetByteArrayElements(pinArray, NULL);
 if (pinPtr == NULL) {
           jniThrowIOException(env, EINVAL);
           env->ReleaseByteArrayElements(address, addr, 0);
 return result;
 }
 }

 int ret = sBluetoothInterface->pin_reply((bt_bdaddr_t*)addr, accept, len,
 (bt_pin_code_t *) pinPtr);
    env->ReleaseByteArrayElements(address, addr, 0);
    env->ReleaseByteArrayElements(pinArray, pinPtr, 0);
    result = (ret == BT_STATUS_SUCCESS) ? JNI_TRUE : JNI_FALSE;

 return result;
}
