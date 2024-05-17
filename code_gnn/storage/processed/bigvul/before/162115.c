void RenderProcessHostImpl::OnChannelConnected(int32_t peer_pid) {
  channel_connected_ = true;
  if (IsReady()) {
    DCHECK(!sent_render_process_ready_);
    sent_render_process_ready_ = true;
    for (auto& observer : observers_)
      observer.RenderProcessReady(this);
  }

#if BUILDFLAG(IPC_MESSAGE_LOG_ENABLED)
  child_control_interface_->SetIPCLoggingEnabled(
      IPC::Logging::GetInstance()->Enabled());
#endif
}
