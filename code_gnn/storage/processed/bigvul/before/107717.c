 double AudioHandler::GetVolumePercent() {
   if (!VerifyMixerConnection())
    return 0;

  return VolumeDbToPercent(mixer_->GetVolumeDb());
 }
