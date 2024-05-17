 void ExternalFeedbackReporterAndroid::ReportExternalFeedback(
  
// void DistillerUIHandleAndroid::ReportExternalFeedback(
      content::WebContents* web_contents,
      const GURL& url,
      const bool good) {
    if (!web_contents)
      return;
  WindowAndroidHelper* helper =
      content::WebContentsUserData<WindowAndroidHelper>::FromWebContents(
          web_contents);
  DCHECK(helper);
  ui::WindowAndroid* window = helper->GetWindowAndroid();
  DCHECK(window);
  
    JNIEnv* env = base::android::AttachCurrentThread();
    ScopedJavaLocalRef<jstring> jurl = base::android::ConvertUTF8ToJavaString(
        env, url_utils::GetOriginalUrlFromDistillerUrl(url).spec());
  
  Java_DomDistillerFeedbackReporter_reportFeedbackWithWindow(
      env, window->GetJavaObject().obj(), jurl.obj(), good);
//   Java_DomDistillerUIUtils_reportFeedbackWithWebContents(
//       env, web_contents->GetJavaWebContents().obj(), jurl.obj(), good);
// }
// 
  
// void DistillerUIHandleAndroid::OpenSettings(
//     content::WebContents* web_contents) {
//   JNIEnv* env = base::android::AttachCurrentThread();
//   Java_DomDistillerUIUtils_openSettings(env,
//       web_contents->GetJavaWebContents().obj());
  }