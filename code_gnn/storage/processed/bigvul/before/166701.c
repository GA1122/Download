void ThreadHeap::PromptlyFreed(size_t gc_info_index) {
  DCHECK(thread_state_->CheckThread());
  size_t entry_index = gc_info_index & kLikelyToBePromptlyFreedArrayMask;
  likely_to_be_promptly_freed_[entry_index] += 3;
}
