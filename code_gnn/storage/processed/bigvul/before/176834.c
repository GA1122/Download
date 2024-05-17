static void adapter_state_change_callback(bt_state_t status) {
 if (!checkCallbackThread()) {
       ALOGE("Callback: '%s' is not called on the correct thread", __FUNCTION__);
 return;
 }
    ALOGV("%s: Status is: %d", __FUNCTION__, status);

    callbackEnv->CallVoidMethod(sJniCallbacksObj, method_stateChangeCallback, (jint)status);

    checkAndClearExceptionFromCallback(callbackEnv, __FUNCTION__);
}
