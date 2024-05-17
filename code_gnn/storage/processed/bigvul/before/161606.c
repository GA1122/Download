String AudioHandler::ChannelInterpretation() {
  switch (new_channel_interpretation_) {
    case AudioBus::kSpeakers:
      return "speakers";
    case AudioBus::kDiscrete:
      return "discrete";
  }
  NOTREACHED();
  return "";
}
