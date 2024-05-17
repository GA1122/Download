static void interopDatabaseClearNative(JNIEnv *env, jobject obj) {
    ALOGV("%s()", __FUNCTION__);
 if (!sBluetoothInterface) return;
    sBluetoothInterface->interop_database_clear();
}
