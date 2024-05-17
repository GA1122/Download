void ImageLoader::SetImageForTest(ImageResourceContent* new_image) {
  DCHECK(new_image);
  SetImageWithoutConsideringPendingLoadEvent(new_image);
}
