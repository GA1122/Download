OfflineAudioDestinationNode::OfflineAudioDestinationNode(
    BaseAudioContext& context,
    unsigned number_of_channels,
    uint32_t frames_to_process,
    float sample_rate)
    : AudioDestinationNode(context) {
  SetHandler(OfflineAudioDestinationHandler::Create(
      *this, number_of_channels, frames_to_process, sample_rate));
}
