void android_net_wifi_hal_cleaned_up_handler(wifi_handle handle) {
    ALOGD("In wifi cleaned up handler");

 JNIHelper helper(mVM);
    helper.setStaticLongField(mCls, WifiHandleVarName, 0);

    helper.deleteGlobalRef(mCls);
    mCls = NULL;
    mVM  = NULL;
}
