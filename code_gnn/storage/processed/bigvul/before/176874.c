static bool set_wake_alarm_callout(uint64_t delay_millis, bool should_wake,
        alarm_cb cb, void *data) {
 JNIEnv *env;
 JavaVM *vm = AndroidRuntime::getJavaVM();
    jint status = vm->GetEnv((void **)&env, JNI_VERSION_1_6);

 if (status != JNI_OK && status != JNI_EDETACHED) {
        ALOGE("%s unable to get environment for JNI call", __func__);
 return false;
 }

 if (status == JNI_EDETACHED && vm->AttachCurrentThread(&env, &sAttachArgs) != 0) {
        ALOGE("%s unable to attach thread to VM", __func__);
 return false;
 }

    sAlarmCallback = cb;
    sAlarmCallbackData = data;

    jboolean jshould_wake = should_wake ? JNI_TRUE : JNI_FALSE;
    jboolean ret = env->CallBooleanMethod(sJniAdapterServiceObj, method_setWakeAlarm,
 (jlong)delay_millis, jshould_wake);
 if (!ret) {
        sAlarmCallback = NULL;
        sAlarmCallbackData = NULL;
 }

 if (status == JNI_EDETACHED) {
        vm->DetachCurrentThread();
 }

 return !!ret;
}
