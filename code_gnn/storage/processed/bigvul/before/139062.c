media::AudioParameters TryToFixAudioParameters(
    const media::AudioParameters& params) {
  DCHECK(!params.IsValid());
  media::AudioParameters params_copy(params);

  if (params.channels() > media::limits::kMaxChannels) {
    DCHECK(params.channel_layout() == media::CHANNEL_LAYOUT_DISCRETE);
    params_copy.set_channels_for_discrete(media::limits::kMaxChannels);
  }

  return params_copy.IsValid()
             ? params_copy
              : media::AudioParameters::UnavailableDeviceParams();
 }
