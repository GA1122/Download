void ChromeInvalidationClient::EmitInvalidation(
    const ObjectIdPayloadMap& id_payloads) {
  DCHECK(CalledOnValidThread());
  listener_->OnInvalidate(id_payloads);
}
