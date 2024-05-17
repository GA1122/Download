void GpuProcessHost::GetProcessHandles(
    const GpuDataManager::GetGpuProcessHandlesCallback& callback)  {
  if (!BrowserThread::CurrentlyOn(BrowserThread::IO)) {
    BrowserThread::PostTask(
        BrowserThread::IO,
        FROM_HERE,
        base::Bind(&GpuProcessHost::GetProcessHandles, callback));
    return;
  }
  std::list<base::ProcessHandle> handles;
  for (int i = 0; i < GPU_PROCESS_KIND_COUNT; ++i) {
    GpuProcessHost* host = g_gpu_process_hosts[i];
    if (host && HostIsValid(host))
      handles.push_back(host->process_->GetHandle());
  }
  BrowserThread::PostTask(
      BrowserThread::UI,
      FROM_HERE,
      base::Bind(callback, handles));
}
