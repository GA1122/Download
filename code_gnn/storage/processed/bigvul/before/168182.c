void UsbTabHelper::IncrementConnectionCount(
    RenderFrameHost* render_frame_host) {
  auto it = frame_usb_services_.find(render_frame_host);
  DCHECK(it != frame_usb_services_.end());
  it->second->device_connection_count_++;
  NotifyTabStateChanged();
}
