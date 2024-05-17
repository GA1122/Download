void AwContents::OnAttachedToWindow(JNIEnv* env, jobject obj, int w, int h) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  browser_view_renderer_.OnAttachedToWindow(w, h);
}
