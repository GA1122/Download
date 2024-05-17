void WebContentsImpl::DispatchBeforeUnload() {
  GetMainFrame()->DispatchBeforeUnload(
      RenderFrameHostImpl::BeforeUnloadType::TAB_CLOSE, false);
}
