OfflineAudioDestinationHandler::Create(AudioNode& node,
                                       unsigned number_of_channels,
                                       uint32_t frames_to_process,
                                       float sample_rate) {
  return base::AdoptRef(new OfflineAudioDestinationHandler(
      node, number_of_channels, frames_to_process, sample_rate));
}
