static void interopDatabaseAddNative(JNIEnv *env, jobject obj, int feature,
                                      jbyteArray address, int length) {
    ALOGV("%s()", __FUNCTION__);
 if (!sBluetoothInterface) return;

    jbyte *addr = env->GetByteArrayElements(address, NULL);
 if (addr == NULL) {
        jniThrowIOException(env, EINVAL);
 return;
 }

    sBluetoothInterface->interop_database_add(feature, (bt_bdaddr_t *)addr, length);
    env->ReleaseByteArrayElements(address, addr, 0);
}
