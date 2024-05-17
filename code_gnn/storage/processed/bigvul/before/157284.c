void WebMediaPlayerImpl::OnVideoDecoderChange(const std::string& name) {
  if (name == video_decoder_name_)
    return;

  video_decoder_name_ = name;

  if (!watch_time_reporter_)
    return;

  UpdateSecondaryProperties();
}
