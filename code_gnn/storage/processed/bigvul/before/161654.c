void AudioNode::disconnect() {
  DCHECK(IsMainThread());
  BaseAudioContext::GraphAutoLocker locker(context());

  for (unsigned i = 0; i < numberOfOutputs(); ++i)
    DisconnectAllFromOutput(i);
}
