static void dumpNative(JNIEnv *env, jobject obj, jobject fdObj)
{
    ALOGV("%s()", __FUNCTION__);
 if (!sBluetoothInterface) return;

 int fd = jniGetFDFromFileDescriptor(env, fdObj);
 if (fd < 0) return;

    sBluetoothInterface->dump(fd);
}
