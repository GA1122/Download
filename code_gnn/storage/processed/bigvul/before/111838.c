bool SyncBackendHost::SetDecryptionPassphrase(const std::string& passphrase) {
  if (!IsNigoriEnabled()) {
    NOTREACHED() << "SetDecryptionPassphrase must never be called when nigori"
                    " is disabled.";
    return false;
  }

  DCHECK(!passphrase.empty());

  DCHECK_EQ(MessageLoop::current(), frontend_loop_);

  DCHECK(cached_pending_keys_.has_blob());

  if (!CheckPassphraseAgainstCachedPendingKeys(passphrase))
    return false;

  sync_thread_.message_loop()->PostTask(FROM_HERE,
      base::Bind(&SyncBackendHost::Core::DoSetDecryptionPassphrase, core_.get(),
                 passphrase));

  NotifyPassphraseAccepted();
  return true;
}
