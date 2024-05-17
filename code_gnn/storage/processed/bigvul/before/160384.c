void LargeObjectPage::makeConsistentForMutator() {
  HeapObjectHeader* header = heapObjectHeader();
  if (header->isMarked())
    header->unmark();
}
