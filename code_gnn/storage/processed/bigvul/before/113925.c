void InvalidationNotifier::OnNotificationsDisabled(
    NotificationsDisabledReason reason) {
  DCHECK(CalledOnValidThread());
  helper_.EmitOnNotificationsDisabled(reason);
}
