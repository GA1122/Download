void UsbTabHelper::CreateChooserService(
    content::RenderFrameHost* render_frame_host,
    mojo::InterfaceRequest<device::mojom::UsbChooserService> request) {
  if (!AllowedByFeaturePolicy(render_frame_host)) {
    mojo::ReportBadMessage(kFeaturePolicyViolation);
    return;
  }
  GetChooserService(render_frame_host, std::move(request));
}
