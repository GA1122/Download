int AudioOutputDeviceTest::CalculateMemorySize(bool synchronized_io) {
  int kInputChannels = synchronized_io ? 2 : 0;

  int output_memory_size =
      AudioBus::CalculateMemorySize(default_audio_parameters_);

  int frames = default_audio_parameters_.frames_per_buffer();
  int input_memory_size =
      AudioBus::CalculateMemorySize(kInputChannels, frames);

  int io_buffer_size = output_memory_size + input_memory_size;

  return TotalSharedMemorySizeInBytes(io_buffer_size);
}
