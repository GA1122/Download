static wifi_interface_handle getIfaceHandle(JNIHelper &helper, jclass cls, jint index) {
 return (wifi_interface_handle) helper.getStaticLongArrayField(cls, WifiIfaceHandleVarName, index);
}
