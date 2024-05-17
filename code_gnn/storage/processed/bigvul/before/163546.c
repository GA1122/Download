void ScriptProcessorHandler::Initialize() {
  if (IsInitialized())
    return;

  float sample_rate = Context()->sampleRate();

  for (unsigned i = 0; i < 2; ++i) {
    AudioBuffer* input_buffer =
        number_of_input_channels_
            ? AudioBuffer::Create(number_of_input_channels_, BufferSize(),
                                  sample_rate)
            : 0;
    AudioBuffer* output_buffer =
        number_of_output_channels_
            ? AudioBuffer::Create(number_of_output_channels_, BufferSize(),
                                  sample_rate)
            : 0;

    input_buffers_.push_back(input_buffer);
    output_buffers_.push_back(output_buffer);
  }

  AudioHandler::Initialize();
}
