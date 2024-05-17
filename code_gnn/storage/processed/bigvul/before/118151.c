void WebContentsAndroid::OnTransitionElementsFetched(
    scoped_ptr<const TransitionLayerData> transition_data,
    bool has_transition_data) {
  DCHECK(has_transition_data);
  JNIEnv* env = AttachCurrentThread();

  std::vector<TransitionElement>::const_iterator it =
      transition_data->elements.begin();
  for (; it != transition_data->elements.end(); ++it) {
    ScopedJavaLocalRef<jstring> jstring_name(ConvertUTF8ToJavaString(env,
                                                                     it->id));
    Java_WebContentsImpl_addNavigationTransitionElements(
        env, obj_.obj(), jstring_name.obj(),
        it->rect.x(), it->rect.y(), it->rect.width(), it->rect.height());
  }

  ScopedJavaLocalRef<jstring> jstring_css_selector(
      ConvertUTF8ToJavaString(env, transition_data->css_selector));
  Java_WebContentsImpl_onTransitionElementsFetched(
      env, obj_.obj(), jstring_css_selector.obj());
}
