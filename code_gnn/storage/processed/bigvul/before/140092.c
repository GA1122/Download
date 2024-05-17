void HTMLMediaElement::setAudioSourceNode(
    AudioSourceProviderClient* sourceNode) {
  DCHECK(isMainThread());
  m_audioSourceNode = sourceNode;

  AudioSourceProviderClientLockScope scope(*this);
  getAudioSourceProvider().setClient(m_audioSourceNode);
}
