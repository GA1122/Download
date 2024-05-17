AudioMixerAlsa::AudioMixerAlsa()
    : min_volume_db_(kDefaultMinVolumeDb),
      max_volume_db_(kDefaultMaxVolumeDb),
      volume_db_(kDefaultVolumeDb),
      is_muted_(false),
      apply_is_pending_(true),
      alsa_mixer_(NULL),
      pcm_element_(NULL),
      prefs_(NULL),
      disconnected_event_(true, false),
      num_connection_attempts_(0) {
}
