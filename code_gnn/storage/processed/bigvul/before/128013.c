void AwContents::InsertVisualStateCallback(
    JNIEnv* env, jobject obj, jlong request_id, jobject callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  ScopedJavaGlobalRef<jobject>* j_callback = new ScopedJavaGlobalRef<jobject>();
  j_callback->Reset(env, callback);
  web_contents_->GetMainFrame()->InsertVisualStateCallback(
      base::Bind(&InvokeVisualStateCallback, java_ref_, request_id,
                 base::Owned(j_callback)));
}
