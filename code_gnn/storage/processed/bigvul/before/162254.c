void CommandBufferProxyImpl::DisconnectChannel() {
  CheckLock();
  if (!channel_ || disconnected_)
    return;
  disconnected_ = true;
  channel_->VerifyFlush(UINT32_MAX);
  channel_->Send(new GpuChannelMsg_DestroyCommandBuffer(route_id_));
  channel_->RemoveRoute(route_id_);
  if (gpu_control_client_)
    gpu_control_client_->OnGpuControlLostContext();
}
