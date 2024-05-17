void RenderWidgetHostImpl::Init() {
  DCHECK(process_->HasConnection());

  renderer_initialized_ = true;

  GpuSurfaceTracker::Get()->SetSurfaceHandle(
      surface_id_, GetCompositingSurface());

  Send(new ViewMsg_CreatingNew_ACK(routing_id_));
  GetProcess()->ResumeRequestsForView(routing_id_);

  WasResized();
}
