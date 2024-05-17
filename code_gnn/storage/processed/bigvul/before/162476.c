void ImageResource::NotifyStartLoad() {
  CHECK_EQ(GetStatus(), ResourceStatus::kPending);
  GetContent()->NotifyStartLoad();
}
