NormalPageArena::NormalPageArena(ThreadState* state, int index)
    : BaseArena(state, index),
      current_allocation_point_(nullptr),
      remaining_allocation_size_(0),
      last_remaining_allocation_size_(0),
      promptly_freed_size_(0),
      is_lazy_sweeping_(false) {
  ClearFreeLists();
}
