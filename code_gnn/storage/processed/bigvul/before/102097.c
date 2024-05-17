void SyncBackendHost::Core::OnUpdatedToken(const std::string& token) {
  host_->frontend_loop_->PostTask(FROM_HERE, NewRunnableMethod(this,
      &Core::NotifyUpdatedToken, token));
}
