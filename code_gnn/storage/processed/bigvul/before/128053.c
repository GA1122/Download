void AwContents::SetJavaPeers(JNIEnv* env,
                              jobject obj,
                              jobject aw_contents,
                              jobject web_contents_delegate,
                              jobject contents_client_bridge,
                              jobject io_thread_client,
                              jobject intercept_navigation_delegate) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  java_ref_ = JavaObjectWeakGlobalRef(env, aw_contents);

  web_contents_delegate_.reset(
      new AwWebContentsDelegate(env, web_contents_delegate));
  web_contents_->SetDelegate(web_contents_delegate_.get());

  contents_client_bridge_.reset(
      new AwContentsClientBridge(env, contents_client_bridge));
  AwContentsClientBridgeBase::Associate(web_contents_.get(),
                                        contents_client_bridge_.get());

  AwContentsIoThreadClientImpl::Associate(
      web_contents_.get(), ScopedJavaLocalRef<jobject>(env, io_thread_client));

  InterceptNavigationDelegate::Associate(
      web_contents_.get(),
      make_scoped_ptr(new InterceptNavigationDelegate(
          env, intercept_navigation_delegate)));

  web_contents_->ForEachFrame(base::Bind(&OnIoThreadClientReady));
}
