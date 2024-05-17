 FrameUsbServices* UsbTabHelper::GetFrameUsbService(
     content::RenderFrameHost* render_frame_host) {
   FrameUsbServicesMap::const_iterator it =
      frame_usb_services_.find(render_frame_host);
  if (it == frame_usb_services_.end()) {
    std::unique_ptr<FrameUsbServices> frame_usb_services(
        new FrameUsbServices());
    it = (frame_usb_services_.insert(
              std::make_pair(render_frame_host, std::move(frame_usb_services))))
             .first;
  }
  return it->second.get();
}
