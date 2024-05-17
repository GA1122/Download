static jboolean android_net_wifi_startHal(JNIEnv* env, jclass cls) {
 JNIHelper helper(env);
    wifi_handle halHandle = getWifiHandle(helper, cls);
 if (halHandle == NULL) {

 if(init_wifi_hal_func_table(&hal_fn) != 0 ) {
            ALOGD("Can not initialize the basic function pointer table");
 return false;
 }

        wifi_error res = init_wifi_vendor_hal_func_table(&hal_fn);
 if (res != WIFI_SUCCESS) {
            ALOGD("Can not initialize the vendor function pointer table");
 return false;
 }

 int ret = set_iface_flags("wlan0", 1);
 if(ret != 0) {
 return false;
 }

        res = hal_fn.wifi_initialize(&halHandle);
 if (res == WIFI_SUCCESS) {
            helper.setStaticLongField(cls, WifiHandleVarName, (jlong)halHandle);
            ALOGD("Did set static halHandle = %p", halHandle);
 }
        env->GetJavaVM(&mVM);
        mCls = (jclass) env->NewGlobalRef(cls);
        ALOGD("halHandle = %p, mVM = %p, mCls = %p", halHandle, mVM, mCls);
 return res == WIFI_SUCCESS;
 } else {
 return (set_iface_flags("wlan0", 1) == 0);
 }
}
