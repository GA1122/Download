void SendGpuProcessMessage(base::WeakPtr<GpuProcessHost> host,
                           IPC::Message* message) {
  if (host)
    host->Send(message);
  else
    delete message;
}
