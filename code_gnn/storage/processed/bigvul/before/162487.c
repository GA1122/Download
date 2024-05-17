void ImageResource::UpdateImageAndClearBuffer() {
  UpdateImage(Data(), ImageResourceContent::kClearAndUpdateImage, true);
  ClearData();
}
