void AwContents::DetachFunctorFromView() {
  JNIEnv* env = AttachCurrentThread();
  ScopedJavaLocalRef<jobject> obj = java_ref_.get(env);
  if (!obj.is_null())
    Java_AwContents_detachFunctorFromView(env, obj.obj());
}
