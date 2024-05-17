static void classInitNative(JNIEnv* env, jclass clazz) {
 int err;
 hw_module_t* module;

    jclass jniCallbackClass =
        env->FindClass("com/android/bluetooth/btservice/JniCallbacks");
    sJniCallbacksField = env->GetFieldID(clazz, "mJniCallbacks",
 "Lcom/android/bluetooth/btservice/JniCallbacks;");

    method_stateChangeCallback = env->GetMethodID(jniCallbackClass, "stateChangeCallback", "(I)V");

    method_adapterPropertyChangedCallback = env->GetMethodID(jniCallbackClass,
 "adapterPropertyChangedCallback",
 "([I[[B)V");
    method_discoveryStateChangeCallback = env->GetMethodID(jniCallbackClass,
 "discoveryStateChangeCallback", "(I)V");

    method_devicePropertyChangedCallback = env->GetMethodID(jniCallbackClass,
 "devicePropertyChangedCallback",
 "([B[I[[B)V");
    method_deviceFoundCallback = env->GetMethodID(jniCallbackClass, "deviceFoundCallback", "([B)V");
    method_pinRequestCallback = env->GetMethodID(jniCallbackClass, "pinRequestCallback",
 "([B[BIZ)V");
    method_sspRequestCallback = env->GetMethodID(jniCallbackClass, "sspRequestCallback",
 "([B[BIII)V");

    method_bondStateChangeCallback = env->GetMethodID(jniCallbackClass,
 "bondStateChangeCallback", "(I[BI)V");

    method_aclStateChangeCallback = env->GetMethodID(jniCallbackClass,
 "aclStateChangeCallback", "(I[BI)V");

    method_setWakeAlarm = env->GetMethodID(clazz, "setWakeAlarm", "(JZ)Z");
    method_acquireWakeLock = env->GetMethodID(clazz, "acquireWakeLock", "(Ljava/lang/String;)Z");
    method_releaseWakeLock = env->GetMethodID(clazz, "releaseWakeLock", "(Ljava/lang/String;)Z");
    method_energyInfo = env->GetMethodID(clazz, "energyInfoCallback", "(IIJJJJ)V");

 char value[PROPERTY_VALUE_MAX];
    property_get("bluetooth.mock_stack", value, "");

 const char *id = (strcmp(value, "1")? BT_STACK_MODULE_ID : BT_STACK_TEST_MODULE_ID);

    err = hw_get_module(id, (hw_module_t const**)&module);

 if (err == 0) {
 hw_device_t* abstraction;
        err = module->methods->open(module, id, &abstraction);
 if (err == 0) {
 bluetooth_module_t* btStack = (bluetooth_module_t *)abstraction;
            sBluetoothInterface = btStack->get_bluetooth_interface();
 } else {
           ALOGE("Error while opening Bluetooth library");
 }
 } else {
        ALOGE("No Bluetooth Library found");
 }
}
