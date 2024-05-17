void ImageResource::DecodeError(bool all_data_received) {
  size_t size = EncodedSize();

  ClearData();
  SetEncodedSize(0);
  if (!ErrorOccurred())
    SetStatus(ResourceStatus::kDecodeError);

  bool is_multipart = !!multipart_parser_;
  if (!all_data_received && Loader()) {
    Loader()->DidFinishLoading(CurrentTimeTicksInSeconds(), size, size, size,
                               false);
  } else {
    auto result = GetContent()->UpdateImage(
        nullptr, GetStatus(),
        ImageResourceContent::kClearImageAndNotifyObservers, all_data_received,
        is_multipart);
    DCHECK_EQ(result, ImageResourceContent::UpdateImageResult::kNoDecodeError);
  }

  GetMemoryCache()->Remove(this);
}
