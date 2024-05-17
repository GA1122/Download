Address ThreadHeap::CheckAndMarkPointer(
    MarkingVisitor* visitor,
    Address address,
    MarkedPointerCallbackForTesting callback) {
  DCHECK(thread_state_->InAtomicMarkingPause());

  if (BasePage* page = LookupPageForAddress(address)) {
    DCHECK(page->Contains(address));
    DCHECK(!address_cache_->Lookup(address));
    DCHECK(&visitor->Heap() == &page->Arena()->GetThreadState()->Heap());
    visitor->ConservativelyMarkAddress(page, address, callback);
    return address;
  }
  if (!address_cache_->Lookup(address))
    address_cache_->AddEntry(address);
  return nullptr;
}
