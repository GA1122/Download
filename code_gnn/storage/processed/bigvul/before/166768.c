void BaseArena::PrepareForSweep() {
  DCHECK(GetThreadState()->InAtomicMarkingPause());
  DCHECK(SweepingCompleted());

  ClearFreeLists();

  VerifyObjectStartBitmap();

  for (BasePage* page = first_page_; page; page = page->Next()) {
    page->MarkAsUnswept();
  }

  first_unswept_page_ = first_page_;
  first_page_ = nullptr;
}
