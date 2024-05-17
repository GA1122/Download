double AudioHandler::PercentToVolumeDb(double volume_percent) const {
  double min_volume_db, max_volume_db;
  mixer_->GetVolumeLimits(&min_volume_db, &max_volume_db);

  return pow(volume_percent / 100.0, kVolumeBias) *
      (max_volume_db - min_volume_db) + min_volume_db;
}
