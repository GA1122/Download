void RenderFrameImpl::BindAutoplayConfiguration(
    blink::mojom::AutoplayConfigurationClientAssociatedRequest request) {
  autoplay_configuration_binding_.Bind(
      std::move(request), GetTaskRunner(blink::TaskType::kInternalIPC));
}
