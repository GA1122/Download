void AudioNode::DisconnectWithoutException(unsigned output_index) {
  DCHECK(IsMainThread());
  BaseAudioContext::GraphAutoLocker locker(context());

  if (output_index >= Handler().NumberOfOutputs())
    return;
  DisconnectAllFromOutput(output_index);
}
