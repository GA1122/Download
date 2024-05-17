BasePage::BasePage(PageMemory* storage, BaseArena* arena)
    : m_storage(storage),
      m_arena(arena),
      m_next(nullptr),
      m_swept(true) {
  ASSERT(isPageHeaderAddress(reinterpret_cast<Address>(this)));
}
