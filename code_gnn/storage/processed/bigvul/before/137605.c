void ChromeDownloadDelegate::SetJavaRef(JNIEnv* env, jobject jobj) {
   java_ref_ = env->NewGlobalRef(jobj);
 }
