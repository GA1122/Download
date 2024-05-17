void ChromeHttpAuthHandler::CancelAuth(JNIEnv* env,
                                       const JavaParamRef<jobject>&) {
  if (observer_)
    observer_->CancelAuth();
}
