void ImageLoader::Dispose() {
  RESOURCE_LOADING_DVLOG(1)
      << "~ImageLoader " << this
      << "; has pending load event=" << pending_load_event_.IsActive()
      << ", has pending error event=" << pending_error_event_.IsActive();

  if (image_content_) {
    image_content_->RemoveObserver(this);
    image_content_ = nullptr;
    image_resource_for_image_document_ = nullptr;
    delay_until_image_notify_finished_ = nullptr;
  }
}
