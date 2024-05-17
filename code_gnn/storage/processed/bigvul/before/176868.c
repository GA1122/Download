int register_com_android_bluetooth_btservice_AdapterService(JNIEnv* env)
{
 return jniRegisterNativeMethods(env, "com/android/bluetooth/btservice/AdapterService",
                                    sMethods, NELEM(sMethods));
}
