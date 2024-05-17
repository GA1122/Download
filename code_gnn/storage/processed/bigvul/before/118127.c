void WebContentsAndroid::ClearNavigationTransitionData(JNIEnv* env,
                                                       jobject jobj) {
  static_cast<WebContentsImpl*>(web_contents_)->ClearNavigationTransitionData();
}
