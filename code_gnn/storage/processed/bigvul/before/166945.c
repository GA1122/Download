MediaStreamAudioSourceNode* BaseAudioContext::createMediaStreamSource(
    MediaStream* media_stream,
    ExceptionState& exception_state) {
  DCHECK(IsMainThread());

  return MediaStreamAudioSourceNode::Create(*this, *media_stream,
                                            exception_state);
}
