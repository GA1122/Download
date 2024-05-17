void HostNPScriptObject::OnReceivedSupportID(
    SupportAccessVerifier* access_verifier,
    bool success,
    const std::string& support_id,
    const base::TimeDelta& lifetime) {
  DCHECK(host_context_.network_message_loop()->BelongsToCurrentThread());

  if (!success) {
    host_context_.main_message_loop()->PostTask(FROM_HERE, base::Bind(
        &HostNPScriptObject::NotifyAccessCode, base::Unretained(this), false));
    DisconnectInternal();
    return;
  }

  access_verifier->OnIT2MeHostRegistered(success, support_id);
  std::string access_code = support_id + access_verifier->host_secret();
  host_->set_access_code(access_code);

  {
    base::AutoLock lock(access_code_lock_);
    access_code_ = access_code;
    access_code_lifetime_ = lifetime;
  }

  host_context_.main_message_loop()->PostTask(FROM_HERE, base::Bind(
      &HostNPScriptObject::NotifyAccessCode, base::Unretained(this), true));
}
