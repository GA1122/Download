static void discovery_state_changed_callback(bt_discovery_state_t state) {
    jbyteArray addr;
 if (!checkCallbackThread()) {
       ALOGE("Callback: '%s' is not called on the correct thread", __FUNCTION__);
 return;
 }

    ALOGV("%s: DiscoveryState:%d ", __FUNCTION__, state);

    callbackEnv->CallVoidMethod(sJniCallbacksObj, method_discoveryStateChangeCallback,
 (jint)state);

    checkAndClearExceptionFromCallback(callbackEnv, __FUNCTION__);
}
