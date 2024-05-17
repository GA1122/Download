void RenderFrameHostImpl::ExecuteJavaScript(const base::string16& javascript,
                                            JavaScriptResultCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  CHECK(CanExecuteJavaScript());

  GetNavigationControl()->JavaScriptExecuteRequest(javascript,
                                                   std::move(callback));
}
