void Instance::ConfigureNumberImageGenerator() {
  std::vector<pp::ImageData> num_images = GetThumbnailResources();
  pp::ImageData number_background = CreateResourceImage(
      PP_RESOURCEIMAGE_PDF_BUTTON_THUMBNAIL_NUM_BACKGROUND);
  number_image_generator_->Configure(number_background,
                                     num_images,
                                     device_scale_);
}
