void InvalidationNotifier::OnInvalidate(const ObjectIdPayloadMap& id_payloads) {
  DCHECK(CalledOnValidThread());
  helper_.DispatchInvalidationsToHandlers(id_payloads, REMOTE_NOTIFICATION);
}
