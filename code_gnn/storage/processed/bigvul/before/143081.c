OfflineAudioDestinationNode* OfflineAudioDestinationNode::Create(
    BaseAudioContext* context,
    unsigned number_of_channels,
    uint32_t frames_to_process,
    float sample_rate) {
  return MakeGarbageCollected<OfflineAudioDestinationNode>(
      *context, number_of_channels, frames_to_process, sample_rate);
}
