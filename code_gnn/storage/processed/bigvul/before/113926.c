void InvalidationNotifier::OnNotificationsEnabled() {
  DCHECK(CalledOnValidThread());
  helper_.EmitOnNotificationsEnabled();
}
