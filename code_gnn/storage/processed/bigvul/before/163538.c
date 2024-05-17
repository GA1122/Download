RefPtr<ScriptProcessorHandler> ScriptProcessorHandler::Create(
    AudioNode& node,
    float sample_rate,
    size_t buffer_size,
    unsigned number_of_input_channels,
    unsigned number_of_output_channels) {
  return WTF::AdoptRef(new ScriptProcessorHandler(
      node, sample_rate, buffer_size, number_of_input_channels,
      number_of_output_channels));
}
