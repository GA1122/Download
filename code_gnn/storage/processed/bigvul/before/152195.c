void RenderFrameImpl::BindFrame(
    const service_manager::BindSourceInfo& browser_info,
    mojom::FrameRequest request) {
  browser_info_ = browser_info;
  frame_binding_.Bind(std::move(request),
                      GetTaskRunner(blink::TaskType::kInternalIPC));
}
