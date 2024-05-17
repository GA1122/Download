AudioHandler::AudioHandler(NodeType node_type,
                           AudioNode& node,
                           float sample_rate)
    : is_initialized_(false),
      node_type_(kNodeTypeUnknown),
      node_(&node),
      context_(node.context()),
      last_processing_time_(-1),
      last_non_silent_time_(-1),
      connection_ref_count_(0),
      is_disabled_(false),
      channel_count_(2) {
  SetNodeType(node_type);
  SetInternalChannelCountMode(kMax);
  SetInternalChannelInterpretation(AudioBus::kSpeakers);

#if DEBUG_AUDIONODE_REFERENCES
  if (!is_node_count_initialized_) {
    is_node_count_initialized_ = true;
    atexit(AudioHandler::PrintNodeCounts);
  }
#endif
  InstanceCounters::IncrementCounter(InstanceCounters::kAudioHandlerCounter);
}
