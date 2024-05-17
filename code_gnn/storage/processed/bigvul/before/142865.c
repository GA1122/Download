void HTMLMediaElement::RemotePlaybackCompatibilityChanged(const WebURL& url,
                                                          bool is_compatible) {
  if (RuntimeEnabledFeatures::NewRemotePlaybackPipelineEnabled() &&
      RemotePlaybackClient()) {
    RemotePlaybackClient()->SourceChanged(url, is_compatible);
  }
}
