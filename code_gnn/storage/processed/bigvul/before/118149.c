void WebContentsAndroid::OnHide(JNIEnv* env, jobject obj) {
  web_contents_->WasHidden();
}
