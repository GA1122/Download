UsbTabHelper::GetPermissionProvider(RenderFrameHost* render_frame_host) {
  FrameUsbServices* frame_usb_services = GetFrameUsbService(render_frame_host);
  if (!frame_usb_services->permission_provider) {
    frame_usb_services->permission_provider.reset(
        new WebUSBPermissionProvider(render_frame_host));
  }
  return frame_usb_services->permission_provider->GetWeakPtr();
}
