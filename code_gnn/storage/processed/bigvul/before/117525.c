void AudioMixerAlsa::SetElementMuted(snd_mixer_elem_t* element, bool mute) {
  DCHECK(MessageLoop::current() == thread_->message_loop());
  int alsa_result = snd_mixer_selem_set_playback_switch_all(element, !mute);
  if (alsa_result != 0) {
    LOG(WARNING) << "snd_mixer_selem_set_playback_switch_all() failed: "
                 << snd_strerror(alsa_result);
  } else {
    VLOG(1) << "Set playback switch " << snd_mixer_selem_get_name(element)
            << " to " << mute;
  }
}
