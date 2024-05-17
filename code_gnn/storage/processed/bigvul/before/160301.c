void ImageLoader::ElementDidMoveToNewDocument() {
  if (delay_until_do_update_from_element_) {
    delay_until_do_update_from_element_->DocumentChanged(
        element_->GetDocument());
  }
  if (delay_until_image_notify_finished_) {
    delay_until_image_notify_finished_->DocumentChanged(
        element_->GetDocument());
  }
  ClearFailedLoadURL();
  ClearImage();
}
