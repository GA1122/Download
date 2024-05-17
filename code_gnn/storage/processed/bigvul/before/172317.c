int register_android_net_wifi_WifiNative(JNIEnv* env) {
 return AndroidRuntime::registerNativeMethods(env,
 "com/android/server/wifi/WifiNative", gWifiMethods, NELEM(gWifiMethods));
}
