void ResourceMessageFilter::OnChannelConnected(int32 peer_pid) {
  DCHECK(!handle()) << " " << handle();
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  base::ProcessHandle peer_handle;
  if (!base::OpenProcessHandle(peer_pid, &peer_handle)) {
    NOTREACHED();
  }
  set_handle(peer_handle);

  audio_renderer_host_->IPCChannelConnected(id(), handle(), this);

  WorkerService::GetInstance()->Initialize(resource_dispatcher_host_);
  appcache_dispatcher_host_->Initialize(this, id(), handle());
  dom_storage_dispatcher_host_->Init(id(), handle());
  db_dispatcher_host_->Init(handle());
}
