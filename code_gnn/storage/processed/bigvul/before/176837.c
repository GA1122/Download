static void callback_thread_event(bt_cb_thread_evt event) {
 JavaVM* vm = AndroidRuntime::getJavaVM();
 if (event  == ASSOCIATE_JVM) {
 JavaVMAttachArgs args;
 char name[] = "BT Service Callback Thread";
        args.version = JNI_VERSION_1_6;
        args.name = name;
        args.group = NULL;
        vm->AttachCurrentThread(&callbackEnv, &args);
        ALOGV("Callback thread attached: %p", callbackEnv);
 } else if (event == DISASSOCIATE_JVM) {
 if (!checkCallbackThread()) {
            ALOGE("Callback: '%s' is not called on the correct thread", __FUNCTION__);
 return;
 }
        vm->DetachCurrentThread();
 }
}
