void RenderFrameImpl::BindFrameNavigationControl(
    mojom::FrameNavigationControlAssociatedRequest request) {
  frame_navigation_control_binding_.Bind(
      std::move(request), GetTaskRunner(blink::TaskType::kInternalIPC));
}
