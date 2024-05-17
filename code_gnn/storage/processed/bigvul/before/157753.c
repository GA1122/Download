void WebContentsImpl::DispatchBeforeUnload(bool auto_cancel) {
  auto before_unload_type =
      auto_cancel ? RenderFrameHostImpl::BeforeUnloadType::DISCARD
                  : RenderFrameHostImpl::BeforeUnloadType::TAB_CLOSE;
  GetMainFrame()->DispatchBeforeUnload(before_unload_type, false);
}
