void AwContents::SetIsPaused(JNIEnv* env, jobject obj, bool paused) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  browser_view_renderer_.SetIsPaused(paused);
  ContentViewCore* cvc =
      ContentViewCore::FromWebContents(web_contents_.get());
  if (cvc) {
    cvc->PauseOrResumeGeolocation(paused);
  }
}
