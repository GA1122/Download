OfflineAudioDestinationNode* OfflineAudioDestinationNode::Create(
    BaseAudioContext* context,
    unsigned number_of_channels,
    size_t frames_to_process,
    float sample_rate) {
  return new OfflineAudioDestinationNode(*context, number_of_channels,
                                         frames_to_process, sample_rate);
}