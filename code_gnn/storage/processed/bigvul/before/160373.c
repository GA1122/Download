bool NormalPage::isEmpty() {
  HeapObjectHeader* header = reinterpret_cast<HeapObjectHeader*>(payload());
  return header->isFree() && header->size() == payloadSize();
}
