BasePage* ThreadHeap::LookupPageForAddress(Address address) {
  DCHECK(thread_state_->InAtomicMarkingPause());
  if (PageMemoryRegion* region = region_tree_->Lookup(address)) {
    return region->PageFromAddress(address);
  }
  return nullptr;
}
