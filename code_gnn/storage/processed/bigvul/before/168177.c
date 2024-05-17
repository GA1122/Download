void UsbTabHelper::DecrementConnectionCount(
    RenderFrameHost* render_frame_host) {
  auto it = frame_usb_services_.find(render_frame_host);
  DCHECK(it != frame_usb_services_.end());
  DCHECK_GT(it->second->device_connection_count_, 0);
  it->second->device_connection_count_--;
  NotifyTabStateChanged();
}
