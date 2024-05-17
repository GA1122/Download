void Instance::CreateProgressBar() {
  pp::ImageData background;
  std::vector<pp::ImageData> images = GetProgressBarResources(&background);
  std::string text = GetLocalizedString(PP_RESOURCESTRING_PDFPROGRESSLOADING);
  progress_bar_.CreateProgressControl(kProgressBarId, false, this, 0.0,
      device_scale_, images, background, text);
}
