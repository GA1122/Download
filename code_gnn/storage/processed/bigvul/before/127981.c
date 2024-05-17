void AwContents::ClearView(JNIEnv* env, jobject obj) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  browser_view_renderer_.ClearView();
}
