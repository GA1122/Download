void SyncBackendHost::Core::OnPassphraseRequired(
    sync_api::PassphraseRequiredReason reason) {
  host_->frontend_loop_->PostTask(FROM_HERE,
      NewRunnableMethod(this, &Core::NotifyPassphraseRequired, reason));
}
