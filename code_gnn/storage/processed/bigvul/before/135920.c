void TextTrackCueList::ValidateCueIndexes() {
  for (size_t i = first_invalid_index_; i < list_.size(); ++i)
    list_[i]->UpdateCueIndex(SafeCast<unsigned>(i));
  first_invalid_index_ = list_.size();
}
