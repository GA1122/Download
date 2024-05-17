MediaElementAudioSourceNode::MediaElementAudioSourceNode(
    AudioContext& context,
    HTMLMediaElement& media_element)
    : AudioNode(context) {
  SetHandler(MediaElementAudioSourceHandler::Create(*this, media_element));
}
