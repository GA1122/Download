void BaseArena::PoisonArena() {
  for (BasePage* page = first_unswept_page_; page; page = page->Next())
    page->PoisonUnmarkedObjects();
}
