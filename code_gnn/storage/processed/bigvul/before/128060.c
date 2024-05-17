void AwContents::SetWindowVisibility(JNIEnv* env, jobject obj, bool visible) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  browser_view_renderer_.SetWindowVisibility(visible);
}
