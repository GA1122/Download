SyncBackendHost::Core::Core(const std::string& name,
                            SyncBackendHost* backend)
    : name_(name),
      host_(backend),
      registrar_(NULL) {
  DCHECK(host_);
}
