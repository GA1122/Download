void SyncBackendHost::RequestNudge(const tracked_objects::Location& location) {
  core_thread_.message_loop()->PostTask(FROM_HERE,
      NewRunnableMethod(core_.get(), &SyncBackendHost::Core::DoRequestNudge,
                        location));
}
