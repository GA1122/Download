void AwContents::OnPermissionRequestCanceled(AwPermissionRequest* request) {
  JNIEnv* env = AttachCurrentThread();
  ScopedJavaLocalRef<jobject> j_request = request->GetJavaObject();
  ScopedJavaLocalRef<jobject> j_ref = java_ref_.get(env);
  if (j_request.is_null() || j_ref.is_null())
    return;

  Java_AwContents_onPermissionRequestCanceled(
      env, j_ref.obj(), j_request.obj());
}
