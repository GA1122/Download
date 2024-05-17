void LargeObjectPage::MakeConsistentForMutator() {
  HeapObjectHeader* header = GetHeapObjectHeader();
  if (header->IsMarked())
    header->Unmark();
}
