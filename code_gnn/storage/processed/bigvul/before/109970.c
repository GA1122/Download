void AudioOutputDevice::AudioThreadCallback::Process(int pending_data) {
  if (pending_data == kPauseMark) {
    memset(shared_memory_.memory(), 0, memory_length_);
    SetActualDataSizeInBytes(&shared_memory_, memory_length_, 0);
    return;
  }

  int audio_delay_milliseconds = pending_data / bytes_per_ms_;

  TRACE_EVENT0("audio", "AudioOutputDevice::FireRenderCallback");

  size_t num_frames = audio_parameters_.frames_per_buffer();

  if (input_bus_.get() && input_channels_ > 0) {
    render_callback_->RenderIO(input_bus_.get(),
                               output_bus_.get(),
                               audio_delay_milliseconds);
  } else {
    num_frames = render_callback_->Render(output_bus_.get(),
                                          audio_delay_milliseconds);
  }

  SetActualDataSizeInBytes(
      &shared_memory_, memory_length_,
      num_frames * sizeof(*output_bus_->channel(0)) * output_bus_->channels());
}
