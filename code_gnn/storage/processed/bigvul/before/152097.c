void RenderFrameHostImpl::SimulateBeforeUnloadAck(bool proceed) {
  DCHECK(is_waiting_for_beforeunload_ack_);
  base::TimeTicks approx_renderer_start_time = send_before_unload_start_time_;

  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE,
      base::BindOnce(&RenderFrameHostImpl::ProcessBeforeUnloadACK,
                     weak_ptr_factory_.GetWeakPtr(), proceed,
                     true  , approx_renderer_start_time,
                     base::TimeTicks::Now()));
}
