void AwContents::TrimMemory(JNIEnv* env,
                            jobject obj,
                            jint level,
                            jboolean visible) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  browser_view_renderer_.TrimMemory(level, visible);
}
