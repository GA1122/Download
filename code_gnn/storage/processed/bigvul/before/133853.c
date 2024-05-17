T* HFSBTreeIterator::GetLeafData() {
  base::CheckedNumeric<size_t> size = sizeof(T);
  auto new_offset = size + current_leaf_offset_;
  if (!new_offset.IsValid() || new_offset.ValueOrDie() >= leaf_data_.size())
    return nullptr;
  T* object = reinterpret_cast<T*>(&leaf_data_[current_leaf_offset_]);
  current_leaf_offset_ = new_offset.ValueOrDie();
  return object;
}
