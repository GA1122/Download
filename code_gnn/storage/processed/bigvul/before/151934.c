void RenderFrameHostImpl::ExecuteJavaScriptForTests(
    const base::string16& javascript,
    JavaScriptResultCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  const bool has_user_gesture = false;
  GetNavigationControl()->JavaScriptExecuteRequestForTests(
      javascript, has_user_gesture, std::move(callback));
}
