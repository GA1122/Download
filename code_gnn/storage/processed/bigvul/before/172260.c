static jboolean android_net_wifi_isDriverLoaded(JNIEnv* env, jobject)
{
 return (::is_wifi_driver_loaded() == 1);
}
