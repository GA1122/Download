static void remote_device_properties_callback(bt_status_t status, bt_bdaddr_t *bd_addr,
 int num_properties, bt_property_t *properties) {
 if (!checkCallbackThread()) {
       ALOGE("Callback: '%s' is not called on the correct thread", __FUNCTION__);
 return;
 }

    ALOGV("%s: Status is: %d, Properties: %d", __FUNCTION__, status, num_properties);

 if (status != BT_STATUS_SUCCESS) {
        ALOGE("%s: Status %d is incorrect", __FUNCTION__, status);
 return;
 }

    callbackEnv->PushLocalFrame(ADDITIONAL_NREFS);

    jobjectArray props;
    jbyteArray addr;
    jintArray types;
    jbyteArray val;
    jclass mclass;

    val = (jbyteArray) callbackEnv->NewByteArray(num_properties);
 if (val == NULL) {
        ALOGE("%s: Error allocating byteArray", __FUNCTION__);
 return;
 }

    mclass = callbackEnv->GetObjectClass(val);

  

    props = callbackEnv->NewObjectArray(num_properties, mclass,
                                             NULL);
 if (props == NULL) {
        ALOGE("%s: Error allocating object Array for properties", __FUNCTION__);
 return;
 }

    types = (jintArray)callbackEnv->NewIntArray(num_properties);

 if (types == NULL) {
        ALOGE("%s: Error allocating int Array for values", __FUNCTION__);
 return;
 }
    callbackEnv->DeleteLocalRef(mclass);
    callbackEnv->DeleteLocalRef(val);

    addr = callbackEnv->NewByteArray(sizeof(bt_bdaddr_t));
 if (addr == NULL) goto Fail;
 if (addr) callbackEnv->SetByteArrayRegion(addr, 0, sizeof(bt_bdaddr_t), (jbyte*)bd_addr);

 if (get_properties(num_properties, properties, &types, &props) < 0) {
 if (props) callbackEnv->DeleteLocalRef(props);
 if (types) callbackEnv->DeleteLocalRef(types);
        callbackEnv->PopLocalFrame(NULL);
 return;
 }

    callbackEnv->CallVoidMethod(sJniCallbacksObj, method_devicePropertyChangedCallback, addr,
                                types, props);
    checkAndClearExceptionFromCallback(callbackEnv, __FUNCTION__);
    callbackEnv->DeleteLocalRef(props);
    callbackEnv->DeleteLocalRef(types);
    callbackEnv->DeleteLocalRef(addr);
    callbackEnv->PopLocalFrame(NULL);
 return;

Fail:
    ALOGE("Error while allocation byte array in %s", __FUNCTION__);
}
