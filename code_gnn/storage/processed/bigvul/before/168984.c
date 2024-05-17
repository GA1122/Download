void CredentialManagerImpl::SendCredential(
    const SendCredentialCallback& send_callback,
    const CredentialInfo& info) {
  DCHECK(pending_request_);
  DCHECK(send_callback.Equals(pending_request_->send_callback()));

  if (password_manager_util::IsLoggingActive(client_)) {
    CredentialManagerLogger(client_->GetLogManager())
        .LogSendCredential(GetLastCommittedURL(), info.type);
  }
  send_callback.Run(info);
  pending_request_.reset();
}
