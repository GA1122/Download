RenderThreadImpl::PendingFrameCreate::PendingFrameCreate(
    const service_manager::BindSourceInfo& browser_info,
    int routing_id,
    mojom::FrameRequest frame_request)
    : browser_info_(browser_info),
      routing_id_(routing_id),
      frame_request_(std::move(frame_request)) {}
