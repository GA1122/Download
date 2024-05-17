BasePage* BaseArena::findPageFromAddress(Address address) {
  for (BasePage* page = m_firstPage; page; page = page->next()) {
    if (page->contains(address))
      return page;
  }
  for (BasePage* page = m_firstUnsweptPage; page; page = page->next()) {
    if (page->contains(address))
      return page;
  }
  return nullptr;
}
