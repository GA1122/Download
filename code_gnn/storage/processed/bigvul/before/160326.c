void ImageLoader::UpdateImageState(ImageResourceContent* new_image_content) {
  image_content_ = new_image_content;
  if (!new_image_content) {
    image_resource_for_image_document_ = nullptr;
    image_complete_ = true;
  } else {
    image_complete_ = false;
  }
  delay_until_image_notify_finished_ = nullptr;
}
