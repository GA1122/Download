void AwContents::PreauthorizePermission(
    JNIEnv* env,
    jobject obj,
    jstring origin,
    jlong resources) {
  permission_request_handler_->PreauthorizePermission(
      GURL(base::android::ConvertJavaStringToUTF8(env, origin)), resources);
}
