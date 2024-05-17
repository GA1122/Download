void ChromeWebContentsDelegateAndroid::OnFindResultAvailable(
    WebContents* web_contents,
    const FindNotificationDetails* find_result) {
  JNIEnv* env = base::android::AttachCurrentThread();
  ScopedJavaLocalRef<jobject> obj = GetJavaDelegate(env);
  if (obj.is_null())
    return;

  ScopedJavaLocalRef<jobject> selection_rect = CreateJavaRect(
      env, find_result->selection_rect());

  ScopedJavaLocalRef<jobject> details_object =
      Java_ChromeWebContentsDelegateAndroid_createFindNotificationDetails(
          env,
          find_result->number_of_matches(),
          selection_rect.obj(),
          find_result->active_match_ordinal(),
          find_result->final_update());

  Java_ChromeWebContentsDelegateAndroid_onFindResultAvailable(
      env,
      obj.obj(),
      details_object.obj());
}
