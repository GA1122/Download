void WebContentsImpl::SetParentNativeViewAccessible(
gfx::NativeViewAccessible accessible_parent) {
  accessible_parent_ = accessible_parent;
  RenderFrameHostImpl* rfh = GetMainFrame();
  if (rfh)
    rfh->SetParentNativeViewAccessible(accessible_parent);
}
