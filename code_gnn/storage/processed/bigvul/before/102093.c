void SyncBackendHost::Core::OnPassphraseAccepted(
    const std::string& bootstrap_token) {
  host_->frontend_loop_->PostTask(FROM_HERE,
      NewRunnableMethod(this, &Core::NotifyPassphraseAccepted,
          bootstrap_token));
}
