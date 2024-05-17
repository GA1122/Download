static int android_net_wifi_getInterfaces(JNIEnv *env, jclass cls) {
 int n = 0;

 JNIHelper helper(env);

    wifi_handle halHandle = getWifiHandle(helper, cls);
    wifi_interface_handle *ifaceHandles = NULL;
 int result = hal_fn.wifi_get_ifaces(halHandle, &n, &ifaceHandles);
 if (result < 0) {
 return result;
 }

 if (n < 0) {
        THROW(helper,"android_net_wifi_getInterfaces no interfaces");
 return 0;
 }

 if (ifaceHandles == NULL) {
       THROW(helper,"android_net_wifi_getInterfaces null interface array");
 return 0;
 }

 if (n > 8) {
        THROW(helper,"Too many interfaces");
 return 0;
 }

    jlongArray array = (env)->NewLongArray(n);
 if (array == NULL) {
        THROW(helper,"Error in accessing array");
 return 0;
 }

    jlong elems[8];
 for (int i = 0; i < n; i++) {
        elems[i] = reinterpret_cast<jlong>(ifaceHandles[i]);
 }

    helper.setLongArrayRegion(array, 0, n, elems);
    helper.setStaticLongArrayField(cls, WifiIfaceHandleVarName, array);

 return (result < 0) ? result : n;
}
