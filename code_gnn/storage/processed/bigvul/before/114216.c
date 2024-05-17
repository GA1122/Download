bool CommandBufferProxyImpl::Echo(const base::Closure& callback) {
  if (last_state_.error != gpu::error::kNoError) {
    return false;
  }

  if (!Send(new GpuCommandBufferMsg_Echo(route_id_,
                    GpuCommandBufferMsg_EchoAck(route_id_)))) {
    return false;
  }

  echo_tasks_.push(callback);

  return true;
}
