void WebContentsAndroid::ExitFullscreen(JNIEnv* env, jobject obj) {
  web_contents_->ExitFullscreen();
}
