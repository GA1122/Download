static void pin_request_callback(bt_bdaddr_t *bd_addr, bt_bdname_t *bdname, uint32_t cod,
 bool min_16_digits) {
    jbyteArray addr, devname;
 if (!checkCallbackThread()) {
       ALOGE("Callback: '%s' is not called on the correct thread", __FUNCTION__);
 return;
 }
 if (!bd_addr) {
        ALOGE("Address is null in %s", __FUNCTION__);
 return;
 }

    addr = callbackEnv->NewByteArray(sizeof(bt_bdaddr_t));
 if (addr == NULL) goto Fail;
    callbackEnv->SetByteArrayRegion(addr, 0, sizeof(bt_bdaddr_t), (jbyte*)bd_addr);

    devname = callbackEnv->NewByteArray(sizeof(bt_bdname_t));
 if (devname == NULL) goto Fail;

    callbackEnv->SetByteArrayRegion(devname, 0, sizeof(bt_bdname_t), (jbyte*)bdname);

    callbackEnv->CallVoidMethod(sJniCallbacksObj, method_pinRequestCallback, addr, devname, cod,
            min_16_digits);

    checkAndClearExceptionFromCallback(callbackEnv, __FUNCTION__);
    callbackEnv->DeleteLocalRef(addr);
    callbackEnv->DeleteLocalRef(devname);
 return;

Fail:
 if (addr) callbackEnv->DeleteLocalRef(addr);
 if (devname) callbackEnv->DeleteLocalRef(devname);
    ALOGE("Error while allocating in: %s", __FUNCTION__);
}
