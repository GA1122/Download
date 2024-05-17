MediaStreamType AdjustAudioStreamTypeBasedOnCommandLineSwitches(
    MediaStreamType stream_type) {
  if (stream_type != MEDIA_GUM_DESKTOP_AUDIO_CAPTURE)
    return stream_type;
  const bool audio_support_flag_for_desktop_share =
      !base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableAudioSupportForDesktopShare);
  return audio_support_flag_for_desktop_share ? MEDIA_GUM_DESKTOP_AUDIO_CAPTURE
                                              : MEDIA_NO_SERVICE;
}
