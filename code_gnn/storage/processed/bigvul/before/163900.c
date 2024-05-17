void ImageCapture::GetMediaTrackSettings(MediaTrackSettings& settings) const {

  if (settings_.hasWhiteBalanceMode())
    settings.setWhiteBalanceMode(settings_.whiteBalanceMode());
  if (settings_.hasExposureMode())
    settings.setExposureMode(settings_.exposureMode());
  if (settings_.hasFocusMode())
    settings.setFocusMode(settings_.focusMode());

  if (settings_.hasPointsOfInterest() &&
      !settings_.pointsOfInterest().IsEmpty()) {
    settings.setPointsOfInterest(settings_.pointsOfInterest());
  }

  if (settings_.hasExposureCompensation())
    settings.setExposureCompensation(settings_.exposureCompensation());
  if (settings_.hasColorTemperature())
    settings.setColorTemperature(settings_.colorTemperature());
  if (settings_.hasIso())
    settings.setIso(settings_.iso());

  if (settings_.hasBrightness())
    settings.setBrightness(settings_.brightness());
  if (settings_.hasContrast())
    settings.setContrast(settings_.contrast());
  if (settings_.hasSaturation())
    settings.setSaturation(settings_.saturation());
  if (settings_.hasSharpness())
    settings.setSharpness(settings_.sharpness());

  if (settings_.hasZoom())
    settings.setZoom(settings_.zoom());
  if (settings_.hasTorch())
    settings.setTorch(settings_.torch());
}
