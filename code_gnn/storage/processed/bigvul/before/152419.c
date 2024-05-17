void RenderFrameImpl::OnBeforeUnload(bool is_reload) {
  TRACE_EVENT1("navigation,rail", "RenderFrameImpl::OnBeforeUnload",
               "id", routing_id_);
  int routing_id = routing_id_;

  base::TimeTicks before_unload_start_time = base::TimeTicks::Now();

  bool proceed = frame_->DispatchBeforeUnloadEvent(is_reload);

  base::TimeTicks before_unload_end_time = base::TimeTicks::Now();
  RenderThread::Get()->Send(new FrameHostMsg_BeforeUnload_ACK(
      routing_id, proceed, before_unload_start_time, before_unload_end_time));
}
