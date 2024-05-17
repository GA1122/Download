BrowserRenderProcessHost::~BrowserRenderProcessHost() {
  WebCacheManager::GetInstance()->Remove(id());
  ChildProcessSecurityPolicy::GetInstance()->Remove(id());

  channel_.reset();

  if (audio_renderer_host_.get())
    audio_renderer_host_->Destroy();

  if (process_.handle() && !run_renderer_in_process()) {
    if (zygote_child_) {
#if defined(OS_LINUX)
      Singleton<ZygoteHost>()->EnsureProcessTerminated(process_.handle());
#endif
    } else {
      ProcessWatcher::EnsureProcessTerminated(process_.handle());
    }
  }

  ClearTransportDIBCache();

  NotificationService::current()->Notify(
      NotificationType::EXTENSION_PORT_DELETED_DEBUG,
      Source<IPC::Message::Sender>(this),
      NotificationService::NoDetails());
}
