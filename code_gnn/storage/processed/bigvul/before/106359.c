void SyncBackendHost::Shutdown(bool sync_disabled) {
  if (core_thread_.IsRunning()) {   
    core_thread_.message_loop()->PostTask(FROM_HERE,
        NewRunnableMethod(core_.get(),
                          &SyncBackendHost::Core::DoShutdown,
                          sync_disabled));
  }

  if (ui_worker())
    ui_worker()->Stop();

  {
    base::ThreadRestrictions::ScopedAllowIO allow_io;
    core_thread_.Stop();
  }

  registrar_.routing_info.clear();
  registrar_.workers[GROUP_DB] = NULL;
  registrar_.workers[GROUP_HISTORY] = NULL;
  registrar_.workers[GROUP_UI] = NULL;
  registrar_.workers[GROUP_PASSIVE] = NULL;
  registrar_.workers[GROUP_PASSWORD] = NULL;
  registrar_.workers.erase(GROUP_DB);
  registrar_.workers.erase(GROUP_HISTORY);
  registrar_.workers.erase(GROUP_UI);
  registrar_.workers.erase(GROUP_PASSIVE);
  registrar_.workers.erase(GROUP_PASSWORD);
  frontend_ = NULL;
  core_ = NULL;   
}
