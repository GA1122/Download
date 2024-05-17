jboolean ForeignSessionHelper::OpenForeignSessionTab(
    JNIEnv* env,
    const JavaParamRef<jobject>& obj,
    const JavaParamRef<jobject>& j_tab,
    const JavaParamRef<jstring>& session_tag,
    jint session_tab_id,
    jint j_disposition) {
  OpenTabsUIDelegate* open_tabs = GetOpenTabsUIDelegate(profile_);
  if (!open_tabs) {
    LOG(ERROR) << "Null OpenTabsUIDelegate returned.";
    return false;
  }

  const sessions::SessionTab* session_tab;

  if (!open_tabs->GetForeignTab(ConvertJavaStringToUTF8(env, session_tag),
                                SessionID::FromSerializedValue(session_tab_id),
                                &session_tab)) {
    LOG(ERROR) << "Failed to load foreign tab.";
    return false;
  }

  if (session_tab->navigations.empty()) {
    LOG(ERROR) << "Foreign tab no longer has valid navigations.";
    return false;
  }

  TabAndroid* tab_android = TabAndroid::GetNativeTab(env, j_tab);
  if (!tab_android)
    return false;
  content::WebContents* web_contents = tab_android->web_contents();
  if (!web_contents)
    return false;

  WindowOpenDisposition disposition =
      static_cast<WindowOpenDisposition>(j_disposition);
  SessionRestore::RestoreForeignSessionTab(web_contents,
                                           *session_tab,
                                           disposition);

  return true;
}
