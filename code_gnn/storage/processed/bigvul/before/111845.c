void SyncBackendHost::StopSyncingForShutdown() {
  DCHECK_EQ(MessageLoop::current(), frontend_loop_);
  if (sync_thread_.IsRunning()) {
    StopSyncManagerForShutdown(
        base::Bind(&SyncBackendRegistrar::OnSyncerShutdownComplete,
                   base::Unretained(registrar_.get())));

    base::Time stop_registrar_start_time = base::Time::Now();
    if (registrar_.get())
      registrar_->StopOnUIThread();
    base::TimeDelta stop_registrar_time = base::Time::Now() -
        stop_registrar_start_time;
    UMA_HISTOGRAM_TIMES("Sync.Shutdown.StopRegistrarTime",
                        stop_registrar_time);
  } else {
    DCHECK_EQ(NOT_ATTEMPTED, initialization_state_);
    DCHECK(!registrar_.get());
  }
}
