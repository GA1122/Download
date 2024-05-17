bool NormalPageArena::IsConsistentForGC() {
  for (size_t i = 0; i < kBlinkPageSizeLog2; ++i) {
    for (FreeListEntry* free_list_entry = free_list_.free_lists_[i];
         free_list_entry; free_list_entry = free_list_entry->Next()) {
      if (PagesToBeSweptContains(free_list_entry->GetAddress()))
        return false;
    }
  }
  if (HasCurrentAllocationArea()) {
    if (PagesToBeSweptContains(CurrentAllocationPoint()))
      return false;
  }
  return true;
}
