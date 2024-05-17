void RenderFrameHostImpl::ExecuteJavaScriptInIsolatedWorld(
    const base::string16& javascript,
    JavaScriptResultCallback callback,
    int world_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK_GT(world_id, ISOLATED_WORLD_ID_GLOBAL);
  DCHECK_LE(world_id, ISOLATED_WORLD_ID_MAX);

  GetNavigationControl()->JavaScriptExecuteRequestInIsolatedWorld(
      javascript, world_id, std::move(callback));
}
