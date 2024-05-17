void BaseArena::MakeConsistentForMutator() {
  ClearFreeLists();
#if DCHECK_IS_ON()
  DCHECK(IsConsistentForGC());
#endif
  DCHECK(!first_page_);

  BasePage* previous_page = nullptr;
  for (BasePage *page = first_unswept_page_; page;
       previous_page = page, page = page->Next()) {
    page->MakeConsistentForMutator();
    page->MarkAsSwept();
  }
  if (previous_page) {
    DCHECK(!SweepingCompleted());
    previous_page->next_ = first_page_;
    first_page_ = first_unswept_page_;
    first_unswept_page_ = nullptr;
  }
  DCHECK(SweepingCompleted());

  VerifyObjectStartBitmap();
}
