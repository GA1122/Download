static jboolean android_net_wifi_startSupplicant(JNIEnv* env, jobject, jboolean p2pSupported)
{
 return (::wifi_start_supplicant(p2pSupported) == 0);
}
