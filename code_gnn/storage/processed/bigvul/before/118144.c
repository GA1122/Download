bool WebContentsAndroid::IsLoading(JNIEnv* env, jobject obj) const {
  return web_contents_->IsLoading();
}
