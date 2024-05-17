void TextTrackCueList::InvalidateCueIndex(size_t index) {
  first_invalid_index_ = std::min(first_invalid_index_, index);
}
