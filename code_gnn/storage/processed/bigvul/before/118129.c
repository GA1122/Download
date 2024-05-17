void WebContentsAndroid::DidDeferAfterResponseStarted(
    const TransitionLayerData& transition_data) {
  JNIEnv* env = AttachCurrentThread();
  std::vector<GURL> entering_stylesheets;
  std::string transition_color;
  if (transition_data.response_headers.get()) {
    TransitionRequestManager::ParseTransitionStylesheetsFromHeaders(
        transition_data.response_headers,
        entering_stylesheets,
        transition_data.request_url);

    transition_data.response_headers->EnumerateHeader(
        NULL, "X-Transition-Entering-Color", &transition_color);
  }

  ScopedJavaLocalRef<jstring> jstring_markup(
      ConvertUTF8ToJavaString(env, transition_data.markup));

  ScopedJavaLocalRef<jstring> jstring_css_selector(
      ConvertUTF8ToJavaString(env, transition_data.css_selector));

  ScopedJavaLocalRef<jstring> jstring_transition_color(
      ConvertUTF8ToJavaString(env, transition_color));

  Java_WebContentsImpl_didDeferAfterResponseStarted(
      env,
      obj_.obj(),
      jstring_markup.obj(),
      jstring_css_selector.obj(),
      jstring_transition_color.obj());

  std::vector<GURL>::const_iterator iter = entering_stylesheets.begin();
  for (; iter != entering_stylesheets.end(); ++iter) {
    ScopedJavaLocalRef<jstring> jstring_url(
        ConvertUTF8ToJavaString(env, iter->spec()));
    Java_WebContentsImpl_addEnteringStylesheetToTransition(
        env, obj_.obj(), jstring_url.obj());
  }
}
