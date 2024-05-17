void HTMLMediaElement::ClearWeakMembers(Visitor* visitor) {
  if (!ThreadHeap::IsHeapObjectAlive(audio_source_node_)) {
    GetAudioSourceProvider().SetClient(nullptr);
    audio_source_node_ = nullptr;
  }
}
