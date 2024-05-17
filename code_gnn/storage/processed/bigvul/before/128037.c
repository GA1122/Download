bool AwContents::RequestDrawGL(bool wait_for_completion) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  JNIEnv* env = AttachCurrentThread();
  ScopedJavaLocalRef<jobject> obj = java_ref_.get(env);
  if (obj.is_null())
    return false;
  return Java_AwContents_requestDrawGL(env, obj.obj(), wait_for_completion);
}
