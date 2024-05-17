bool AudioMixerAlsa::GetElementVolume(snd_mixer_elem_t* element,
                                      double* current_volume_db) {
  DCHECK(MessageLoop::current() == thread_->message_loop());
  alsa_long_t long_volume = 0;
  int alsa_result = snd_mixer_selem_get_playback_dB(
      element, static_cast<snd_mixer_selem_channel_id_t>(0), &long_volume);
  if (alsa_result != 0) {
    LOG(WARNING) << "snd_mixer_selem_get_playback_dB() failed: "
                 << snd_strerror(alsa_result);
    return false;
  }
  *current_volume_db = static_cast<double>(long_volume) / 100.0;
  return true;
}
