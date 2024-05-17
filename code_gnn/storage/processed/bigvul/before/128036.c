jlong AwContents::ReleasePopupAwContents(JNIEnv* env, jobject obj) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  return reinterpret_cast<intptr_t>(pending_contents_.release());
}
