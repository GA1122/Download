bool NormalPageArena::pagesToBeSweptContains(Address address) {
  for (BasePage* page = m_firstUnsweptPage; page; page = page->next()) {
    if (page->contains(address))
      return true;
  }
  return false;
}
