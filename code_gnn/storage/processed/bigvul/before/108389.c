bool DeinterleaveAudioChannel(void* source,
                              float* destination,
                              int channels,
                              int channel_index,
                              int bytes_per_sample,
                              size_t number_of_frames) {
  switch (bytes_per_sample) {
    case 1:
    {
      uint8* source8 = static_cast<uint8*>(source) + channel_index;
      const float kScale = 1.0f / 128.0f;
      for (unsigned i = 0; i < number_of_frames; ++i) {
        destination[i] = kScale * static_cast<int>(*source8 + 128);
        source8 += channels;
      }
      return true;
    }

    case 2:
    {
      int16* source16 = static_cast<int16*>(source) + channel_index;
      const float kScale = 1.0f / 32768.0f;
      for (unsigned i = 0; i < number_of_frames; ++i) {
        destination[i] = kScale * *source16;
        source16 += channels;
      }
      return true;
    }

    case 4:
    {
      int32* source32 = static_cast<int32*>(source) + channel_index;
      const float kScale = 1.0f / (1L << 31);
      for (unsigned i = 0; i < number_of_frames; ++i) {
        destination[i] = kScale * *source32;
        source32 += channels;
      }
      return true;
    }

    default:
     break;
  }
  return false;
}
