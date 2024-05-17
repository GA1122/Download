MediaElementAudioSourceHandler::Create(AudioNode& node,
                                       HTMLMediaElement& media_element) {
  return base::AdoptRef(
      new MediaElementAudioSourceHandler(node, media_element));
}
