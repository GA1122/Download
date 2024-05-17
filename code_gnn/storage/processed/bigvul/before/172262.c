static jboolean android_net_wifi_killSupplicant(JNIEnv* env, jobject, jboolean p2pSupported)
{
 return (::wifi_stop_supplicant(p2pSupported) == 0);
}
