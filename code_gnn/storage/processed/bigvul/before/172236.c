jint Java_com_android_server_wifi_WifiNative_registerNatives(JNIEnv* env, jclass clazz) {
 return AndroidRuntime::registerNativeMethods(env,
 "com/android/server/wifi/WifiNative", gWifiMethods, NELEM(gWifiMethods));
}
