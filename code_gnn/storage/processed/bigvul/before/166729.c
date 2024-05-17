void NormalPageArena::ClearFreeLists() {
  SetAllocationPoint(nullptr, 0);
  free_list_.Clear();
  promptly_freed_size_ = 0;
}
