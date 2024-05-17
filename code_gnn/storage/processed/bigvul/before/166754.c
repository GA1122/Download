void NormalPage::MakeConsistentForMutator() {
  object_start_bit_map()->Clear();
  Address start_of_gap = Payload();
  NormalPageArena* normal_arena = ArenaForNormalPage();
  for (Address header_address = Payload(); header_address < PayloadEnd();) {
    HeapObjectHeader* header =
        reinterpret_cast<HeapObjectHeader*>(header_address);
    size_t size = header->size();
    DCHECK_LT(size, BlinkPagePayloadSize());
    if (header->IsFree()) {
      SET_MEMORY_INACCESSIBLE(header_address, size < sizeof(FreeListEntry)
                                                  ? size
                                                  : sizeof(FreeListEntry));
      CHECK_MEMORY_INACCESSIBLE(header_address, size);
      header_address += size;
      continue;
    }
    if (start_of_gap != header_address)
      normal_arena->AddToFreeList(start_of_gap, header_address - start_of_gap);
    if (header->IsMarked()) {
      header->Unmark();
    }
    object_start_bit_map()->SetBit(header_address);
    header_address += size;
    start_of_gap = header_address;
    DCHECK_LE(header_address, PayloadEnd());
  }
  if (start_of_gap != PayloadEnd())
    normal_arena->AddToFreeList(start_of_gap, PayloadEnd() - start_of_gap);

  VerifyObjectStartBitmapIsConsistentWithPayload();
}
