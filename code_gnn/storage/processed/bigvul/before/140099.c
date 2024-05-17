void HTMLMediaElement::setMediaStreamRegistry(URLRegistry* registry) {
  DCHECK(!s_mediaStreamRegistry);
  s_mediaStreamRegistry = registry;
}
