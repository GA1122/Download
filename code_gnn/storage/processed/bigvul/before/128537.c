void Instance::CreatePageIndicator(bool always_visible) {
  page_indicator_.CreatePageIndicator(kPageIndicatorId, false, this,
      number_image_generator(), always_visible);
  ConfigurePageIndicator();
}
