static int release_wake_lock_callout(const char *lock_name) {
 JNIEnv *env;
 JavaVM *vm = AndroidRuntime::getJavaVM();
    jint status = vm->GetEnv((void **)&env, JNI_VERSION_1_6);

 if (status != JNI_OK && status != JNI_EDETACHED) {
        ALOGE("%s unable to get environment for JNI call", __func__);
 return BT_STATUS_FAIL;
 }
 if (status == JNI_EDETACHED && vm->AttachCurrentThread(&env, &sAttachArgs) != 0) {
        ALOGE("%s unable to attach thread to VM", __func__);
 return BT_STATUS_FAIL;
 }
    jboolean ret = JNI_FALSE;
    jstring lock_name_jni = env->NewStringUTF(lock_name);
 if (lock_name_jni) {
        ret = env->CallBooleanMethod(sJniAdapterServiceObj, method_releaseWakeLock, lock_name_jni);
        env->DeleteLocalRef(lock_name_jni);
 } else {
        ALOGE("%s unable to allocate string: %s", __func__, lock_name);
 }
 if (status == JNI_EDETACHED) {
        vm->DetachCurrentThread();
 }
 return ret ? BT_STATUS_SUCCESS : BT_STATUS_FAIL;
}
