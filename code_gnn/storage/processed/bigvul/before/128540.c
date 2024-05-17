void Instance::CreateThumbnails() {
  thumbnails_.CreateThumbnailControl(
      kThumbnailsId, pp::Rect(), false, this, engine_.get(),
      number_image_generator());
}
