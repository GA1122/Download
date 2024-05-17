void AwContents::Destroy(JNIEnv* env, jobject obj) {
  java_ref_.reset();
  delete this;
}
