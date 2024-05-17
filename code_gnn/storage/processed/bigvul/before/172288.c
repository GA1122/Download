static jboolean android_net_wifi_unloadDriver(JNIEnv* env, jobject)
{
 return (::wifi_unload_driver() == 0);
}
