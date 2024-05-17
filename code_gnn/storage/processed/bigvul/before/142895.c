void HTMLMediaElement::SetAudioSourceNode(
    AudioSourceProviderClient* source_node) {
  DCHECK(IsMainThread());
  audio_source_node_ = source_node;

  GetAudioSourceProvider().SetClient(audio_source_node_);
}
