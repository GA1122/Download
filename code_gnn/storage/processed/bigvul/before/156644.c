  void CommitText(const char* text) {
    JNIEnv* env = base::android::AttachCurrentThread();

    base::android::ScopedJavaLocalRef<jobject> caller =
        ime_adapter()->java_ime_adapter_for_testing(env);

    base::android::ScopedJavaLocalRef<jstring> jtext =
        base::android::ConvertUTF8ToJavaString(env, text);

    ime_adapter()->CommitText(
        env, base::android::JavaParamRef<jobject>(env, caller.obj()),
        base::android::JavaParamRef<jobject>(env, jtext.obj()),
        base::android::JavaParamRef<jstring>(env, jtext.obj()), 0);
  }
