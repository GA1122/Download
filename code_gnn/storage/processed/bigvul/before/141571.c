void ForeignSessionHelper::DeleteForeignSession(
    JNIEnv* env,
    const JavaParamRef<jobject>& obj,
    const JavaParamRef<jstring>& session_tag) {
  OpenTabsUIDelegate* open_tabs = GetOpenTabsUIDelegate(profile_);
  if (open_tabs)
    open_tabs->DeleteForeignSession(ConvertJavaStringToUTF8(env, session_tag));
}
