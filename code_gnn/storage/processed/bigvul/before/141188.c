void Document::ProcessJavaScriptUrl(
    const KURL& url,
    ContentSecurityPolicyDisposition disposition) {
  DCHECK(url.ProtocolIsJavaScript());
  if (frame_->Loader().StateMachine()->IsDisplayingInitialEmptyDocument())
    load_event_progress_ = kLoadEventNotRun;
  frame_->Loader().Progress().ProgressStarted();
  if (frame_->Loader().StateMachine()->IsDisplayingInitialEmptyDocument() &&
      (url == "javascript:''" || url == "javascript:\"\"")) {
    ExecuteJavaScriptUrl(url, disposition);
    return;
  }
  javascript_url_task_handle_ = PostCancellableTask(
      *GetTaskRunner(TaskType::kNetworking), FROM_HERE,
      WTF::Bind(&Document::ExecuteJavaScriptUrl, WrapWeakPersistent(this), url,
                disposition));
}
