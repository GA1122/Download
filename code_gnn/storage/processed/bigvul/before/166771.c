void BaseArena::RemoveAllPages() {
  ClearFreeLists();

  DCHECK(SweepingCompleted());
  while (first_page_) {
    BasePage* page = first_page_;
    page->Unlink(&first_page_);
    page->RemoveFromHeap();
  }
}
