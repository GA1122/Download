NormalPageArena::NormalPageArena(ThreadState* state, int index)
    : BaseArena(state, index),
      m_currentAllocationPoint(nullptr),
      m_remainingAllocationSize(0),
      m_lastRemainingAllocationSize(0),
      m_promptlyFreedSize(0),
      m_isLazySweeping(false) {
  clearFreeLists();
}
