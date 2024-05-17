void WebBluetoothServiceImpl::DidFinishNavigation(
    NavigationHandle* navigation_handle) {
  if (navigation_handle->HasCommitted() &&
      navigation_handle->GetRenderFrameHost() == render_frame_host_ &&
      !navigation_handle->IsSameDocument()) {
    ClearState();
  }
}
