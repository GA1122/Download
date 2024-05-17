void ImageResource::FlushImageIfNeeded(TimerBase*) {
  if (IsLoading()) {
    last_flush_time_ = WTF::CurrentTimeTicksInSeconds();
    UpdateImage(Data(), ImageResourceContent::kUpdateImage, false);
  }
}
