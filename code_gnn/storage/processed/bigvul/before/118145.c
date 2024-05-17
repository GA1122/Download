bool WebContentsAndroid::IsLoadingToDifferentDocument(JNIEnv* env,
                                                      jobject obj) const {
  return web_contents_->IsLoadingToDifferentDocument();
}
