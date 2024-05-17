void BaseArena::poisonArena() {
  for (BasePage* page = m_firstUnsweptPage; page; page = page->next())
    page->poisonUnmarkedObjects();
}
