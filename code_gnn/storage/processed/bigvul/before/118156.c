void WebContentsAndroid::RevertExitTransition(JNIEnv* env,
                                              jobject jobj) {
  web_contents_->GetMainFrame()->Send(new FrameMsg_RevertExitTransition(
      web_contents_->GetMainFrame()->GetRoutingID()));
}
