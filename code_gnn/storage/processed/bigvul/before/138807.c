void RenderFrameHostImpl::SimulateBeforeUnloadAck() {
  DCHECK(is_waiting_for_beforeunload_ack_);
  base::TimeTicks approx_renderer_start_time = send_before_unload_start_time_;
  OnBeforeUnloadACK(true, approx_renderer_start_time, base::TimeTicks::Now());
}
