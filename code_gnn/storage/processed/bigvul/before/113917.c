void ChromeInvalidationClient::OnNotificationsEnabled() {
  DCHECK(CalledOnValidThread());
  push_client_state_ = NO_NOTIFICATION_ERROR;
  EmitStateChange();
}
