void SendGpuProcessMessage(GpuProcessHost::GpuProcessKind kind,
                           content::CauseForGpuLaunch cause,
                           IPC::Message* message) {
  GpuProcessHost* host = GpuProcessHost::Get(kind, cause);
  if (host) {
    host->Send(message);
  } else {
    delete message;
  }
}
