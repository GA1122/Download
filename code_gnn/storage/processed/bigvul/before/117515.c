bool AudioMixerAlsa::ConnectInternal() {
  DCHECK(MessageLoop::current() == thread_->message_loop());
  num_connection_attempts_++;
  int err;
  snd_mixer_t* handle = NULL;

  if ((err = snd_mixer_open(&handle, 0)) < 0) {
    if (num_connection_attempts_ == kConnectionAttemptToLogFailure)
      LOG(WARNING) << "Mixer open error: " << snd_strerror(err);
    return false;
  }

  if ((err = snd_mixer_attach(handle, kCardName)) < 0) {
    if (num_connection_attempts_ == kConnectionAttemptToLogFailure)
      LOG(WARNING) << "Attach to card " << kCardName << " failed: "
                   << snd_strerror(err);
    snd_mixer_close(handle);
    return false;
  }

  snd_pcm_t* pcm_out_handle;
  if ((err = snd_pcm_open(&pcm_out_handle,
                          kCardName,
                          SND_PCM_STREAM_PLAYBACK,
                          0)) >= 0) {
    snd_pcm_close(pcm_out_handle);
  } else {
    if (num_connection_attempts_ == kConnectionAttemptToLogFailure)
      LOG(WARNING) << "PCM open failed: " << snd_strerror(err);
  }

  if ((err = snd_mixer_selem_register(handle, NULL, NULL)) < 0) {
    if (num_connection_attempts_ == kConnectionAttemptToLogFailure)
      LOG(WARNING) << "Mixer register error: " << snd_strerror(err);
    snd_mixer_close(handle);
    return false;
  }

  if ((err = snd_mixer_load(handle)) < 0) {
    if (num_connection_attempts_ == kConnectionAttemptToLogFailure)
      LOG(WARNING) << "Mixer " << kCardName << " load error: %s"
                   << snd_strerror(err);
    snd_mixer_close(handle);
    return false;
  }

  VLOG(1) << "Opened mixer " << kCardName << " successfully";

  double min_volume_db = kDefaultMinVolumeDb;
  double max_volume_db = kDefaultMaxVolumeDb;

  snd_mixer_elem_t* master_element = NULL;
  for (size_t i = 0; i < arraysize(kMasterElementNames); ++i) {
    master_element = FindElementWithName(handle, kMasterElementNames[i]);
    if (master_element)
      break;
  }

  if (!master_element) {
    if (num_connection_attempts_ == kConnectionAttemptToLogFailure)
      LOG(WARNING) << "Unable to find a master element on " << kCardName;
    snd_mixer_close(handle);
    return false;
  }

  alsa_long_t long_low = static_cast<alsa_long_t>(kDefaultMinVolumeDb * 100);
  alsa_long_t long_high = static_cast<alsa_long_t>(kDefaultMaxVolumeDb * 100);
  err = snd_mixer_selem_get_playback_dB_range(
      master_element, &long_low, &long_high);
  if (err != 0) {
    if (num_connection_attempts_ == kConnectionAttemptToLogFailure)
      LOG(WARNING) << "snd_mixer_selem_get_playback_dB_range() failed:"
                   << snd_strerror(err);
    snd_mixer_close(handle);
    return false;
  }
  min_volume_db = static_cast<double>(long_low) / 100.0;
  max_volume_db = static_cast<double>(long_high) / 100.0;

  snd_mixer_elem_t* pcm_element = FindElementWithName(handle, kPCMElementName);
  if (pcm_element) {
    alsa_long_t long_low = static_cast<alsa_long_t>(kDefaultMinVolumeDb * 100);
    alsa_long_t long_high = static_cast<alsa_long_t>(kDefaultMaxVolumeDb * 100);
    err = snd_mixer_selem_get_playback_dB_range(
        pcm_element, &long_low, &long_high);
    if (err != 0) {
      if (num_connection_attempts_ == kConnectionAttemptToLogFailure)
        LOG(WARNING) << "snd_mixer_selem_get_playback_dB_range() failed for "
                     << kPCMElementName << ": " << snd_strerror(err);
      snd_mixer_close(handle);
      return false;
    }
    min_volume_db += static_cast<double>(long_low) / 100.0;
    max_volume_db += static_cast<double>(long_high) / 100.0;
  }

  VLOG(1) << "Volume range is " << min_volume_db << " dB to "
          << max_volume_db << " dB";
  {
    base::AutoLock lock(lock_);
    alsa_mixer_ = handle;
    master_element_ = master_element;
    pcm_element_ = pcm_element;
    min_volume_db_ = min_volume_db;
    max_volume_db_ = max_volume_db;
    volume_db_ = min(max(volume_db_, min_volume_db_), max_volume_db_);
  }

  EnableChromeOsTts();

  ApplyState();
  return true;
}
