static jboolean android_net_wifi_toggle_interface(JNIEnv* env, jclass cls, int toggle) {
 return(set_iface_flags("wlan0", toggle) == 0);
}
