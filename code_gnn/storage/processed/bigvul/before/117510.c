double AudioHandler::VolumeDbToPercent(double volume_db) const {
  double min_volume_db, max_volume_db;
  mixer_->GetVolumeLimits(&min_volume_db, &max_volume_db);

  if (volume_db < min_volume_db)
    return 0.0;
  return 100.0 * pow((volume_db - min_volume_db) /
      (max_volume_db - min_volume_db), 1/kVolumeBias);
}
