bool AwContents::IsVisible(JNIEnv* env, jobject obj) {
  return browser_view_renderer_.IsClientVisible();
}
