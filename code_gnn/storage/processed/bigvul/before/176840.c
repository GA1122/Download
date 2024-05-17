void checkAndClearExceptionFromCallback(JNIEnv* env,
 const char* methodName) {
 if (env->ExceptionCheck()) {
        ALOGE("An exception was thrown by callback '%s'.", methodName);
        LOGE_EX(env);
        env->ExceptionClear();
 }
}
