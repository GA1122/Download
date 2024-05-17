void AudioRendererAlgorithm::CrossfadeFrame(
    uint8* outtro_bytes, const uint8* intro_bytes) {
  Type* outtro = reinterpret_cast<Type*>(outtro_bytes);
  const Type* intro = reinterpret_cast<const Type*>(intro_bytes);

  int frames_in_crossfade = bytes_in_crossfade_ / bytes_per_frame_;
  float crossfade_ratio =
      static_cast<float>(crossfade_frame_number_) / frames_in_crossfade;
  for (int channel = 0; channel < channels_; ++channel) {
    *outtro *= 1.0 - crossfade_ratio;
    *outtro++ += (*intro++) * crossfade_ratio;
  }
  crossfade_frame_number_++;
}
