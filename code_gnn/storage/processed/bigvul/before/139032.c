void ImageLoader::setImage(ImageResourceContent* newImage) {
  setImageWithoutConsideringPendingLoadEvent(newImage);

  updatedHasPendingEvent();
}
