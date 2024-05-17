void WebContentsAndroid::SetHasPendingNavigationTransitionForTesting(
    JNIEnv* env,
    jobject obj) {
  base::CommandLine::ForCurrentProcess()->AppendSwitch(
      switches::kEnableExperimentalWebPlatformFeatures);
  RenderFrameHost* frame =
      static_cast<WebContentsImpl*>(web_contents_)->GetMainFrame();
  BrowserThread::PostTask(
      BrowserThread::IO,
      FROM_HERE,
      base::Bind(
          &TransitionRequestManager::AddPendingTransitionRequestDataForTesting,
          base::Unretained(TransitionRequestManager::GetInstance()),
          frame->GetProcess()->GetID(),
          frame->GetRoutingID()));
}
