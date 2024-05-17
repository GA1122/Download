MediaElementAudioSourceNode* BaseAudioContext::createMediaElementSource(
    HTMLMediaElement* media_element,
    ExceptionState& exception_state) {
  DCHECK(IsMainThread());

  return MediaElementAudioSourceNode::Create(*this, *media_element,
                                             exception_state);
}
