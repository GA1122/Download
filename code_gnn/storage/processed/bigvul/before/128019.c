void AwContents::OnDetachedFromWindow(JNIEnv* env, jobject obj) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  browser_view_renderer_.OnDetachedFromWindow();
}