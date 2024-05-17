void WebMediaPlayerImpl::OnAudioDecoderChange(const std::string& name) {
  if (name == audio_decoder_name_)
    return;

  audio_decoder_name_ = name;

  if (!watch_time_reporter_)
    return;

  UpdateSecondaryProperties();
}
