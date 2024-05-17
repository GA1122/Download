void WebContentsAndroid::OnShow(JNIEnv* env, jobject obj) {
  web_contents_->WasShown();
}
