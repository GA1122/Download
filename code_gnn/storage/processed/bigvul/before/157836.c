void WebContentsImpl::ReloadLoFiImages() {
  SendToAllFrames(new FrameMsg_ReloadLoFiImages(MSG_ROUTING_NONE));
  for (auto& observer : observers_)
    observer.DidReloadLoFiImages();
}
