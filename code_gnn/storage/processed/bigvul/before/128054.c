void AwContents::SetJsOnlineProperty(JNIEnv* env,
                                     jobject obj,
                                     jboolean network_up) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  render_view_host_ext_->SetJsOnlineProperty(network_up);
}
