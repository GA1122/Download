void GpuChannel::OnChannelError() {
   gpu_channel_manager_->RemoveChannel(client_id_);
 }
