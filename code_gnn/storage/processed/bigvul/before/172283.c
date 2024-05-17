static void android_net_wifi_stopHal(JNIEnv* env, jclass cls) {
    ALOGD("In wifi stop Hal");

 JNIHelper helper(env);
    wifi_handle halHandle = getWifiHandle(helper, cls);
 if (halHandle == NULL)
 return;

    ALOGD("halHandle = %p, mVM = %p, mCls = %p", halHandle, mVM, mCls);
    hal_fn.wifi_cleanup(halHandle, android_net_wifi_hal_cleaned_up_handler);
}
