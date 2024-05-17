NotificationsDisabledReason ChromeInvalidationClient::GetState() const {
  DCHECK(CalledOnValidThread());
  if (ticl_state_ == NOTIFICATION_CREDENTIALS_REJECTED ||
      push_client_state_ == NOTIFICATION_CREDENTIALS_REJECTED) {
    return NOTIFICATION_CREDENTIALS_REJECTED;
  }
  if (ticl_state_ == NO_NOTIFICATION_ERROR &&
      push_client_state_ == NO_NOTIFICATION_ERROR) {
    return NO_NOTIFICATION_ERROR;
  }
  return TRANSIENT_NOTIFICATION_ERROR;
}
