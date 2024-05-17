MediaElementAudioSourceNode* MediaElementAudioSourceNode::Create(
    AudioContext* context,
    const MediaElementAudioSourceOptions& options,
    ExceptionState& exception_state) {
  return Create(*context, *options.mediaElement(), exception_state);
}
