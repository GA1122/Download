void GpuProcessHost::SendOnIO(GpuProcessKind kind,
                              content::CauseForGpuLaunch cause,
                              IPC::Message* message) {
  BrowserThread::PostTask(
        BrowserThread::IO, FROM_HERE,
        base::Bind(
            &SendGpuProcessMessage, kind, cause, message));
}
