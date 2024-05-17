void ChromotingInstance::HandleVideoControl(const base::DictionaryValue& data) {
  protocol::VideoControl video_control;
  bool pause_video = false;
  if (data.GetBoolean("pause", &pause_video)) {
    video_control.set_enable(!pause_video);
  }
  bool lossless_encode = false;
  if (data.GetBoolean("losslessEncode", &lossless_encode)) {
    video_control.set_lossless_encode(lossless_encode);
  }
  bool lossless_color = false;
  if (data.GetBoolean("losslessColor", &lossless_color)) {
    video_control.set_lossless_color(lossless_color);
  }
  if (!IsConnected()) {
    return;
  }
  client_->host_stub()->ControlVideo(video_control);
}
