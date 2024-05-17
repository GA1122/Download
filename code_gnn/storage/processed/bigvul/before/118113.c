void ChromeWebContentsDelegateAndroid::FindMatchRectsReply(
    WebContents* web_contents,
    int version,
    const std::vector<gfx::RectF>& rects,
    const gfx::RectF& active_rect) {
  JNIEnv* env = base::android::AttachCurrentThread();
  ScopedJavaLocalRef<jobject> obj = GetJavaDelegate(env);
  if (obj.is_null())
    return;

  ScopedJavaLocalRef<jobject> details_object =
      Java_ChromeWebContentsDelegateAndroid_createFindMatchRectsDetails(
          env,
          version,
          rects.size(),
          CreateJavaRectF(env, active_rect).obj());

  for (size_t i = 0; i < rects.size(); ++i) {
      Java_ChromeWebContentsDelegateAndroid_setMatchRectByIndex(
          env,
          details_object.obj(),
          i,
          CreateJavaRectF(env, rects[i]).obj());
  }

  Java_ChromeWebContentsDelegateAndroid_onFindMatchRectsAvailable(
      env,
      obj.obj(),
      details_object.obj());
}
