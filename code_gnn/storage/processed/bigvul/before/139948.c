void HTMLMediaElement::clearWeakMembers(Visitor* visitor) {
  if (!ThreadHeap::isHeapObjectAlive(m_audioSourceNode)) {
    getAudioSourceProvider().setClient(nullptr);
    m_audioSourceNode = nullptr;
  }
}
