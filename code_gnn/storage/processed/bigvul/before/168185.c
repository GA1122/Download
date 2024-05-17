void UsbTabHelper::RenderFrameDeleted(RenderFrameHost* render_frame_host) {
  frame_usb_services_.erase(render_frame_host);
   NotifyTabStateChanged();
 }
