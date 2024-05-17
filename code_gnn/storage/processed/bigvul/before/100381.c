void BrowserRenderProcessHost::OnChannelConnected(int32 peer_pid) {
  if (!process_.handle()) {
    if (fast_shutdown_started_) {
      return;
    } else if (base::GetCurrentProcId() == peer_pid) {
      process_.set_handle(base::GetCurrentProcessHandle());
    } else {
#if defined(OS_WIN)
      process_.set_handle(OpenProcess(MAXIMUM_ALLOWED, FALSE, peer_pid));
#else
      NOTREACHED();
#endif
      DCHECK(process_.handle());
    }
  } else {
    if (peer_pid != process_.pid()) {
#if !defined(OS_LINUX)
      CHECK(peer_pid == process_.pid()) << peer_pid << " " << process_.pid();
#endif
    }
    mark_child_process_activity_time();
  }

#if defined(IPC_MESSAGE_LOG_ENABLED)
  bool enabled = IPC::Logging::current()->Enabled();
  Send(new ViewMsg_SetIPCLoggingEnabled(enabled));
#endif

}
