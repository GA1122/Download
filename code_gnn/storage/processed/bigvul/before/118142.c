void WebContentsAndroid::InsertCSS(
    JNIEnv* env, jobject jobj, jstring jcss) {
  web_contents_->InsertCSS(base::android::ConvertJavaStringToUTF8(env, jcss));
}
