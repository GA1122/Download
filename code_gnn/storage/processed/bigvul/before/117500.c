void AudioHandler::AdjustVolumeByPercent(double adjust_by_percent) {
  const double old_volume_db = mixer_->GetVolumeDb();
  const double old_percent = VolumeDbToPercent(old_volume_db);
  SetVolumePercent(old_percent + adjust_by_percent);
}
