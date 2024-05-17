ScopedJavaLocalRef<jstring> WebContentsAndroid::GetURL(JNIEnv* env,
                                                       jobject obj) const {
  return ConvertUTF8ToJavaString(env, web_contents_->GetURL().spec());
}
