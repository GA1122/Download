void BaseArena::SweepUnsweptPage() {
  BasePage* page = first_unswept_page_;
  if (page->IsEmpty()) {
    page->Unlink(&first_unswept_page_);
    page->RemoveFromHeap();
  } else {
    page->Sweep();
    page->Unlink(&first_unswept_page_);
    page->Link(&first_page_);
    page->MarkAsSwept();
  }
}
