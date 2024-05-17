void WebContentsImpl::OnAdvanceFocus(RenderFrameHostImpl* source_rfh) {
  if (GetOuterWebContents() &&
      GetOuterWebContents() == source_rfh->delegate()->GetAsWebContents() &&
      GetFocusedWebContents() == GetOuterWebContents()) {
    SetAsFocusedWebContentsIfNecessary();
  }
}
