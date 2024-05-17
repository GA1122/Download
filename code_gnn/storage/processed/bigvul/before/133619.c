void WebContentsImpl::RenderViewDeleted(RenderViewHost* rvh) {
  ClearPowerSaveBlockers(rvh);
  GetRenderManager()->RenderViewDeleted(rvh);
  FOR_EACH_OBSERVER(WebContentsObserver, observers_, RenderViewDeleted(rvh));
}
