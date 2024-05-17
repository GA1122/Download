bool AudioMixerAlsa::SetElementVolume(snd_mixer_elem_t* element,
                                      double new_volume_db,
                                      double rounding_bias) {
  DCHECK(MessageLoop::current() == thread_->message_loop());
  alsa_long_t volume_low = 0;
  alsa_long_t volume_high = 0;
  int alsa_result = snd_mixer_selem_get_playback_volume_range(
      element, &volume_low, &volume_high);
  if (alsa_result != 0) {
    LOG(WARNING) << "snd_mixer_selem_get_playback_volume_range() failed: "
                 << snd_strerror(alsa_result);
    return false;
  }
  alsa_long_t volume_range = volume_high - volume_low;
  if (volume_range <= 0)
    return false;

  alsa_long_t db_low_int = 0;
  alsa_long_t db_high_int = 0;
  alsa_result =
      snd_mixer_selem_get_playback_dB_range(element, &db_low_int, &db_high_int);
  if (alsa_result != 0) {
    LOG(WARNING) << "snd_mixer_selem_get_playback_dB_range() failed: "
                 << snd_strerror(alsa_result);
    return false;
  }

  double db_low = static_cast<double>(db_low_int) / 100.0;
  double db_high = static_cast<double>(db_high_int) / 100.0;
  double db_step = static_cast<double>(db_high - db_low) / volume_range;
  if (db_step <= 0.0)
    return false;

  if (new_volume_db < db_low)
    new_volume_db = db_low;

  alsa_long_t value = static_cast<alsa_long_t>(
      rounding_bias + (new_volume_db - db_low) / db_step) + volume_low;
  alsa_result = snd_mixer_selem_set_playback_volume_all(element, value);
  if (alsa_result != 0) {
    LOG(WARNING) << "snd_mixer_selem_set_playback_volume_all() failed: "
                 << snd_strerror(alsa_result);
    return false;
  }

  VLOG(1) << "Set volume " << snd_mixer_selem_get_name(element)
          << " to " << new_volume_db << " ==> "
          << (value - volume_low) * db_step + db_low << " dB";

  return true;
}
