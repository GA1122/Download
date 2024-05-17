size_t NormalPage::ObjectPayloadSizeForTesting() {
  size_t object_payload_size = 0;
  Address header_address = Payload();
  DCHECK_NE(header_address, PayloadEnd());
  do {
    HeapObjectHeader* header =
        reinterpret_cast<HeapObjectHeader*>(header_address);
    if (!header->IsFree()) {
      object_payload_size += header->PayloadSize();
    }
    DCHECK_LT(header->size(), BlinkPagePayloadSize());
    header_address += header->size();
    DCHECK_LE(header_address, PayloadEnd());
  } while (header_address < PayloadEnd());
  return object_payload_size;
}
