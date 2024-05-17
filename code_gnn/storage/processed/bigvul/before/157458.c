void ChromeHttpAuthHandler::Init() {
  DCHECK(java_chrome_http_auth_handler_.is_null());
  JNIEnv* env = AttachCurrentThread();
  java_chrome_http_auth_handler_.Reset(
      Java_ChromeHttpAuthHandler_create(env, reinterpret_cast<intptr_t>(this)));
}
