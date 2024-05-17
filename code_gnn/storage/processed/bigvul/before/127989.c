void AwContents::EnableOnNewPicture(JNIEnv* env,
                                    jobject obj,
                                    jboolean enabled) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  browser_view_renderer_.EnableOnNewPicture(enabled);
}
