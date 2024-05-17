void FreeList::Clear() {
  biggest_free_list_index_ = 0;
  for (size_t i = 0; i < kBlinkPageSizeLog2; ++i)
    free_lists_[i] = nullptr;
}
