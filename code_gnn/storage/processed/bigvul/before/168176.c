void UsbTabHelper::CreateDeviceManager(
    RenderFrameHost* render_frame_host,
    mojo::InterfaceRequest<device::mojom::UsbDeviceManager> request) {
  if (!AllowedByFeaturePolicy(render_frame_host)) {
    mojo::ReportBadMessage(kFeaturePolicyViolation);
    return;
  }
  device::usb::DeviceManagerImpl::Create(
      GetPermissionProvider(render_frame_host), std::move(request));
}
