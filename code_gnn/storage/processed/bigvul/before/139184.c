void RenderProcessHostImpl::OnChannelConnected(int32_t peer_pid) {
  channel_connected_ = true;
  if (IsReady()) {
    DCHECK(!sent_render_process_ready_);
    sent_render_process_ready_ = true;
    for (auto& observer : observers_)
      observer.RenderProcessReady(this);
  }

#if defined(IPC_MESSAGE_LOG_ENABLED)
  Send(new ChildProcessMsg_SetIPCLoggingEnabled(
      IPC::Logging::GetInstance()->Enabled()));
#endif

  tracked_objects::ThreadData::Status status =
      tracked_objects::ThreadData::status();
  Send(new ChildProcessMsg_SetProfilerStatus(status));

  BrowserThread::PostTask(BrowserThread::IO, FROM_HERE,
                          base::Bind(&AudioInputRendererHost::set_renderer_pid,
                                     audio_input_renderer_host_, peer_pid));
}
