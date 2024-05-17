static wifi_handle getWifiHandle(JNIHelper &helper, jclass cls) {
 return (wifi_handle) helper.getStaticLongField(cls, WifiHandleVarName);
}
