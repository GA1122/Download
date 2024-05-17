void SyncBackendHost::Shutdown(bool sync_disabled) {
  if (sync_thread_.IsRunning()) {
    sync_thread_.message_loop()->PostTask(FROM_HERE,
        base::Bind(&SyncBackendHost::Core::DoShutdown, core_.get(),
                   sync_disabled));
  }

  base::Time stop_thread_start_time = base::Time::Now();
  {
    base::ThreadRestrictions::ScopedAllowIO allow_io;
    sync_thread_.Stop();
  }
  base::TimeDelta stop_sync_thread_time = base::Time::Now() -
      stop_thread_start_time;
  UMA_HISTOGRAM_TIMES("Sync.Shutdown.StopSyncThreadTime",
                      stop_sync_thread_time);

  registrar_.reset();
  frontend_ = NULL;
  core_ = NULL;   
}
