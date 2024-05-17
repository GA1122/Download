void RenderProcessHostImpl::OnProcessLaunched() {
  if (deleting_soon_)
    return;

  if (child_process_launcher_) {
    DCHECK(child_process_launcher_->GetProcess().IsValid());
    DCHECK(!is_process_backgrounded_);

    channel_->Unpause(false  );

    if (child_connection_) {
      child_connection_->SetProcessHandle(
          child_process_launcher_->GetProcess().Handle());
    }

#if defined(OS_MACOSX)
    is_process_backgrounded_ =
        child_process_launcher_->GetProcess().IsProcessBackgrounded(
            MachBroker::GetInstance());
#else
    is_process_backgrounded_ =
        child_process_launcher_->GetProcess().IsProcessBackgrounded();
#endif   

#if defined(OS_ANDROID)
    UpdateProcessPriority();
#endif

    CreateSharedRendererHistogramAllocator();
  }

  NotificationService::current()->Notify(NOTIFICATION_RENDERER_PROCESS_CREATED,
                                         Source<RenderProcessHost>(this),
                                         NotificationService::NoDetails());

  if (child_process_launcher_)
    channel_->Flush();

  if (IsReady()) {
    DCHECK(!sent_render_process_ready_);
    sent_render_process_ready_ = true;
    for (auto& observer : observers_)
      observer.RenderProcessReady(this);
  }

#if BUILDFLAG(ENABLE_WEBRTC)
  if (WebRTCInternals::GetInstance()->IsAudioDebugRecordingsEnabled()) {
    EnableAudioDebugRecordings(
        WebRTCInternals::GetInstance()->GetAudioDebugRecordingsFilePath());
  }
#endif
}
