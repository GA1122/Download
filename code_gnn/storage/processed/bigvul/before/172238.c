static void android_net_wifi_closeSupplicantConnection(JNIEnv* env, jobject)
{
 ::wifi_close_supplicant_connection();
}
