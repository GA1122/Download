void Document::ExecuteJavaScriptUrl(
    const KURL& url,
    ContentSecurityPolicyDisposition disposition) {
  if (!frame_)
    return;
  frame_->GetScriptController().ExecuteScriptIfJavaScriptURL(url, nullptr,
                                                             disposition);
  CheckCompleted();
}
