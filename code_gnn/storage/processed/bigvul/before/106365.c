void SyncBackendHost::UpdateCredentials(const SyncCredentials& credentials) {
  core_thread_.message_loop()->PostTask(FROM_HERE,
      NewRunnableMethod(core_.get(),
                        &SyncBackendHost::Core::DoUpdateCredentials,
                        credentials));
}
