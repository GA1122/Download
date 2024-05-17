void RenderFrameHostImpl::OnBeforeUnloadACK(
    bool proceed,
    const base::TimeTicks& renderer_before_unload_start_time,
    const base::TimeTicks& renderer_before_unload_end_time) {
  ProcessBeforeUnloadACK(proceed, false  ,
                         renderer_before_unload_start_time,
                         renderer_before_unload_end_time);
}