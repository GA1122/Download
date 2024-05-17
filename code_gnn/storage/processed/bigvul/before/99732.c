size_t VP9Decoder::GetRequiredNumOfPictures() const {
  return media::limits::kMaxVideoFrames + media::kVp9NumRefFrames + 2;
}
