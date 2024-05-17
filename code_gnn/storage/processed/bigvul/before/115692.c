void HostNPScriptObject::OnAccessDenied() {
  DCHECK(host_context_.network_message_loop()->BelongsToCurrentThread());

  ++failed_login_attempts_;
  if (failed_login_attempts_ == kMaxLoginAttempts)
    DisconnectInternal();
 }
