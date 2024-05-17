static jlong Init(JNIEnv* env,
                  jclass clazz,
                  jobject obj,
                  jobject java_web_contents) {
  content::WebContents* web_contents =
      content::WebContents::FromJavaWebContents(java_web_contents);

  return reinterpret_cast<intptr_t>(
      new ConnectionInfoPopupAndroid(env, obj, web_contents));
}
