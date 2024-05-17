void ChromeHttpAuthHandler::ShowDialog(const JavaRef<jobject>& window_android) {
  JNIEnv* env = AttachCurrentThread();
  Java_ChromeHttpAuthHandler_showDialog(env, java_chrome_http_auth_handler_,
                                         window_android);
 }
