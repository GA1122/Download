void AudioNode::HandleChannelOptions(const AudioNodeOptions& options,
                                     ExceptionState& exception_state) {
  DCHECK(IsMainThread());

  if (options.hasChannelCount())
    setChannelCount(options.channelCount(), exception_state);
  if (options.hasChannelCountMode())
    setChannelCountMode(options.channelCountMode(), exception_state);
  if (options.hasChannelInterpretation())
    setChannelInterpretation(options.channelInterpretation(), exception_state);
}
