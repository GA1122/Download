void WebContentsAndroid::Stop(JNIEnv* env, jobject obj) {
  web_contents_->Stop();
}
