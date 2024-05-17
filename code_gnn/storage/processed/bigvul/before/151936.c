void RenderFrameHostImpl::ExecuteJavaScriptWithUserGestureForTests(
    const base::string16& javascript) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  const bool has_user_gesture = true;
  GetNavigationControl()->JavaScriptExecuteRequestForTests(
      javascript, has_user_gesture, base::NullCallback());
}
