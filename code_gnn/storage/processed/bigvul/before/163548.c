ScriptProcessorHandler::ScriptProcessorHandler(
    AudioNode& node,
    float sample_rate,
    size_t buffer_size,
    unsigned number_of_input_channels,
    unsigned number_of_output_channels)
    : AudioHandler(kNodeTypeScriptProcessor, node, sample_rate),
      double_buffer_index_(0),
      buffer_size_(buffer_size),
      buffer_read_write_index_(0),
      number_of_input_channels_(number_of_input_channels),
      number_of_output_channels_(number_of_output_channels),
      internal_input_bus_(AudioBus::Create(number_of_input_channels,
                                           AudioUtilities::kRenderQuantumFrames,
                                           false)) {
  if (buffer_size_ < AudioUtilities::kRenderQuantumFrames)
    buffer_size_ = AudioUtilities::kRenderQuantumFrames;

  DCHECK_LE(number_of_input_channels, BaseAudioContext::MaxNumberOfChannels());

  AddInput();
  AddOutput(number_of_output_channels);

  channel_count_ = number_of_input_channels;
  SetInternalChannelCountMode(kExplicit);

  Initialize();
}
