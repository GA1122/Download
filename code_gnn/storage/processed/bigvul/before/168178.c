void UsbTabHelper::GetChooserService(
    content::RenderFrameHost* render_frame_host,
    mojo::InterfaceRequest<device::mojom::UsbChooserService> request) {
  FrameUsbServices* frame_usb_services = GetFrameUsbService(render_frame_host);
  if (!frame_usb_services->chooser_service) {
    frame_usb_services->chooser_service.reset(
#if defined(OS_ANDROID)
        new WebUsbChooserServiceAndroid(render_frame_host));
#else
        new WebUsbChooserServiceDesktop(render_frame_host));
#endif   
  }
  frame_usb_services->chooser_service->Bind(std::move(request));
}
