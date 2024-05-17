void AudioOutputDevice::AudioThreadCallback::MapSharedMemory() {
  shared_memory_.Map(TotalSharedMemorySizeInBytes(memory_length_));

  int output_memory_size = AudioBus::CalculateMemorySize(audio_parameters_);
  int frames = audio_parameters_.frames_per_buffer();
  int input_memory_size =
      AudioBus::CalculateMemorySize(input_channels_, frames);

  int io_size = output_memory_size + input_memory_size;

  DCHECK_EQ(memory_length_, io_size);

  output_bus_ =
      AudioBus::WrapMemory(audio_parameters_, shared_memory_.memory());

  if (input_channels_ > 0) {
    char* input_data =
        static_cast<char*>(shared_memory_.memory()) + output_memory_size;
    input_bus_ =
        AudioBus::WrapMemory(input_channels_, frames, input_data);
  }
}
