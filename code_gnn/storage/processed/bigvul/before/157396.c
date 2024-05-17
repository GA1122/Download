void HTMLMediaElement::SetMediaStreamRegistry(URLRegistry* registry) {
  DCHECK(!media_stream_registry_);
  media_stream_registry_ = registry;
}
