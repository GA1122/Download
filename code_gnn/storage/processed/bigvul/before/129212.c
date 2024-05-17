BackRenderbuffer::BackRenderbuffer(
    RenderbufferManager* renderbuffer_manager,
    MemoryTracker* memory_tracker,
    ContextState* state)
    : renderbuffer_manager_(renderbuffer_manager),
      memory_tracker_(memory_tracker, MemoryTracker::kUnmanaged),
      state_(state),
      bytes_allocated_(0),
      id_(0) {
}
