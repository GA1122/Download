void EglRenderingVDAClient::GetRangeForNextNALUs(
    size_t start_pos, size_t* end_pos) {
  *end_pos = start_pos;
  CHECK(LookingAtNAL(encoded_data_, start_pos));
  for (int i = 0; i < num_NALUs_per_decode_; ++i) {
    *end_pos += 4;
    while (*end_pos + 3 < encoded_data_.size() &&
           !LookingAtNAL(encoded_data_, *end_pos)) {
      ++*end_pos;
    }
    if (*end_pos + 3 >= encoded_data_.size()) {
      *end_pos = encoded_data_.size();
      return;
    }
  }
}
