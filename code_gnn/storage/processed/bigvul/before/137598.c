void Init(JNIEnv* env,
          const JavaParamRef<jobject>& obj,
          const JavaParamRef<jobject>& jweb_contents) {
  auto* web_contents = WebContents::FromJavaWebContents(jweb_contents);
  ChromeDownloadDelegate::CreateForWebContents(web_contents);
  ChromeDownloadDelegate::FromWebContents(web_contents)->SetJavaRef(env, obj);
}
