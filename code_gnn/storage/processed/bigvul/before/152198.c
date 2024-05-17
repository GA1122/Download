void RenderFrameImpl::BindFullscreen(
    mojom::FullscreenVideoElementHandlerAssociatedRequest request) {
  fullscreen_binding_.Bind(std::move(request),
                           GetTaskRunner(blink::TaskType::kInternalIPC));
}
