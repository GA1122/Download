gfx::Point AwContents::GetLocationOnScreen() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  JNIEnv* env = AttachCurrentThread();
  ScopedJavaLocalRef<jobject> obj = java_ref_.get(env);
  if (obj.is_null())
    return gfx::Point();
  std::vector<int> location;
  base::android::JavaIntArrayToIntVector(
      env,
      Java_AwContents_getLocationOnScreen(env, obj.obj()).obj(),
      &location);
   return gfx::Point(location[0], location[1]);
 }
