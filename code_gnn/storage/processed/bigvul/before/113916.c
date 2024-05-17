void ChromeInvalidationClient::OnNotificationsDisabled(
    notifier::NotificationsDisabledReason reason) {
  DCHECK(CalledOnValidThread());
  push_client_state_ = FromNotifierReason(reason);
  EmitStateChange();
}
