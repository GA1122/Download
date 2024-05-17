static void android_net_wifi_waitForHalEvents(JNIEnv* env, jclass cls) {

    ALOGD("waitForHalEvents called, vm = %p, obj = %p, env = %p", mVM, mCls, env);

 JNIHelper helper(env);
    wifi_handle halHandle = getWifiHandle(helper, cls);
    hal_fn.wifi_event_loop(halHandle);
    set_iface_flags("wlan0", 0);
}
