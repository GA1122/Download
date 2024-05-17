AudioRendererAlgorithm::AudioRendererAlgorithm()
    : channels_(0),
      samples_per_second_(0),
      bytes_per_channel_(0),
      playback_rate_(0.0f),
      audio_buffer_(0, kStartingBufferSizeInBytes),
      bytes_in_crossfade_(0),
      bytes_per_frame_(0),
      index_into_window_(0),
      crossfade_frame_number_(0),
      muted_(false),
      needs_more_data_(false),
      window_size_(0) {
}
