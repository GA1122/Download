void ChromeInvalidationClient::EmitStateChange() {
  DCHECK(CalledOnValidThread());
  if (GetState() == NO_NOTIFICATION_ERROR) {
    listener_->OnNotificationsEnabled();
  } else {
    listener_->OnNotificationsDisabled(GetState());
  }
}
