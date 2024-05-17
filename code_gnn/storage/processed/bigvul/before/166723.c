BaseArena::BaseArena(ThreadState* state, int index)
    : first_page_(nullptr),
      first_unswept_page_(nullptr),
      thread_state_(state),
      index_(index) {}
