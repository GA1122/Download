void WebContentsAndroid::SetupTransitionView(JNIEnv* env,
                                             jobject jobj,
                                             jstring markup) {
  web_contents_->GetMainFrame()->Send(new FrameMsg_SetupTransitionView(
      web_contents_->GetMainFrame()->GetRoutingID(),
      ConvertJavaStringToUTF8(env, markup)));
}
