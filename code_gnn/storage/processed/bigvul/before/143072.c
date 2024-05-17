void DefaultAudioDestinationHandler::SetChannelCount(
    unsigned channel_count,
    ExceptionState& exception_state) {
  DCHECK(IsMainThread());

  if (channel_count > MaxChannelCount()) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kIndexSizeError,
        ExceptionMessages::IndexOutsideRange<unsigned>(
            "channel count", channel_count, 1,
            ExceptionMessages::kInclusiveBound, MaxChannelCount(),
            ExceptionMessages::kInclusiveBound));
    return;
  }

  unsigned long old_channel_count = this->ChannelCount();
  AudioHandler::SetChannelCount(channel_count, exception_state);

  if (this->ChannelCount() != old_channel_count &&
      !exception_state.HadException()) {
    StopPlatformDestination();
    CreatePlatformDestination();
    StartPlatformDestination();
  }
}
