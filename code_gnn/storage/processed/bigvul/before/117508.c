void AudioHandler::SetVolumePercent(double volume_percent) {
  volume_percent = min(max(volume_percent, 0.0), 100.0);
  mixer_->SetVolumeDb(PercentToVolumeDb(volume_percent));
}
