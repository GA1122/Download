void WebContentsAndroid::ResumeResponseDeferredAtStart(JNIEnv* env,
                                                       jobject obj) {
  static_cast<WebContentsImpl*>(web_contents_)->ResumeResponseDeferredAtStart();
}
