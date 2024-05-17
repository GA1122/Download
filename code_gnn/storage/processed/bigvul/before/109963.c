void AudioOutputDevice::InitializeIO(const AudioParameters& params,
                                     int input_channels,
                                     RenderCallback* callback) {
  DCHECK_GE(input_channels, 0);
  DCHECK_LT(input_channels, limits::kMaxChannels);
  input_channels_ = input_channels;
  Initialize(params, callback);
}
