jboolean WebContentsAndroid::IsIncognito(JNIEnv* env, jobject obj) {
  return web_contents_->GetBrowserContext()->IsOffTheRecord();
}
