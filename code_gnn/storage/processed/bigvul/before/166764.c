bool NormalPageArena::PagesToBeSweptContains(Address address) {
  for (BasePage* page = first_unswept_page_; page; page = page->Next()) {
    if (page->Contains(address))
      return true;
  }
  return false;
}
