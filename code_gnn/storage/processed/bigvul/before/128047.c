void AwContents::SetBackgroundColor(JNIEnv* env, jobject obj, jint color) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  render_view_host_ext_->SetBackgroundColor(color);
}
