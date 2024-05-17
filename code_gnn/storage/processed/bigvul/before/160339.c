BaseArena::BaseArena(ThreadState* state, int index)
    : m_firstPage(nullptr),
      m_firstUnsweptPage(nullptr),
      m_threadState(state),
      m_index(index) {}
