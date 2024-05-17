void InvokeVisualStateCallback(const JavaObjectWeakGlobalRef& java_ref,
                               jlong request_id,
                               ScopedJavaGlobalRef<jobject>* callback,
                               bool result) {
  JNIEnv* env = AttachCurrentThread();
  ScopedJavaLocalRef<jobject> obj = java_ref.get(env);
  if (obj.is_null())
     return;
  Java_AwContents_invokeVisualStateCallback(
      env, obj.obj(), callback->obj(), request_id);
}
