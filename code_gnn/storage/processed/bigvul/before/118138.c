ScopedJavaLocalRef<jstring> WebContentsAndroid::GetTitle(
    JNIEnv* env, jobject obj) const {
  return base::android::ConvertUTF16ToJavaString(env,
                                                 web_contents_->GetTitle());
}
