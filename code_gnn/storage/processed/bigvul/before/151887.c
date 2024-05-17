void RenderFrameHostImpl::BindSerialServiceRequest(
    blink::mojom::SerialServiceRequest request) {
  if (!IsFeatureEnabled(blink::mojom::FeaturePolicyFeature::kSerial)) {
    mojo::ReportBadMessage("Feature policy blocks access to Serial.");
    return;
  }

  if (!serial_service_)
    serial_service_ = std::make_unique<SerialService>(this);

  serial_service_->Bind(std::move(request));
}
