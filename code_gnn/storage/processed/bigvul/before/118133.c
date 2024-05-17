void WebContentsAndroid::FetchTransitionElements(JNIEnv* env,
                                                 jobject jobj,
                                                 jstring jurl) {
  GURL url(base::android::ConvertJavaStringToUTF8(env, jurl));
  RenderFrameHost* frame = web_contents_->GetMainFrame();

  scoped_ptr<TransitionLayerData> transition_data(new TransitionLayerData());
  BrowserThread::PostTaskAndReplyWithResult(
      BrowserThread::IO,
      FROM_HERE,
      base::Bind(&TransitionRequestManager::GetPendingTransitionRequest,
                 base::Unretained(TransitionRequestManager::GetInstance()),
                 frame->GetProcess()->GetID(),
                 frame->GetRoutingID(),
                 url,
                 transition_data.get()),
      base::Bind(&WebContentsAndroid::OnTransitionElementsFetched,
                 weak_factory_.GetWeakPtr(),
                 base::Passed(&transition_data)));
}
