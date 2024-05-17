static jboolean android_net_wifi_connectToSupplicant(JNIEnv* env, jobject)
{
 return (::wifi_connect_to_supplicant() == 0);
}
