void WebContentsAndroid::BeginExitTransition(JNIEnv* env,
                                             jobject jobj,
                                             jstring css_selector,
                                             jboolean exit_to_native_app) {
  web_contents_->GetMainFrame()->Send(new FrameMsg_BeginExitTransition(
      web_contents_->GetMainFrame()->GetRoutingID(),
      ConvertJavaStringToUTF8(env, css_selector),
      exit_to_native_app));
}
