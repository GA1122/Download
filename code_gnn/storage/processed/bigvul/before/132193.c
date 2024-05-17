void RenderFrameImpl::DidCommitCompositorFrame() {
  if (BrowserPluginManager::Get())
    BrowserPluginManager::Get()->DidCommitCompositorFrame(GetRoutingID());
  FOR_EACH_OBSERVER(
      RenderFrameObserver, observers_, DidCommitCompositorFrame());
}
