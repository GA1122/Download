void MediaElementAudioSourceNode::Trace(blink::Visitor* visitor) {
  AudioSourceProviderClient::Trace(visitor);
  AudioNode::Trace(visitor);
}
