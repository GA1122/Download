static jboolean android_net_wifi_is_get_channels_for_band_supported(JNIEnv *env, jclass cls){
 return (hal_fn.wifi_get_valid_channels == wifi_get_valid_channels_stub);
}
