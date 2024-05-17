void Instance::ConfigurePageIndicator() {
  pp::ImageData background =
      CreateResourceImage(PP_RESOURCEIMAGE_PDF_PAGE_INDICATOR_BACKGROUND);
  page_indicator_.Configure(pp::Point(), background);
}
