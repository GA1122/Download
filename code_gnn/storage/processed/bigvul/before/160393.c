void NormalPage::populateObjectStartBitMap() {
  memset(&m_objectStartBitMap, 0, objectStartBitMapSize);
  Address start = payload();
  for (Address headerAddress = start; headerAddress < payloadEnd();) {
    HeapObjectHeader* header =
        reinterpret_cast<HeapObjectHeader*>(headerAddress);
    size_t objectOffset = headerAddress - start;
    ASSERT(!(objectOffset & allocationMask));
    size_t objectStartNumber = objectOffset / allocationGranularity;
    size_t mapIndex = objectStartNumber / 8;
    ASSERT(mapIndex < objectStartBitMapSize);
    m_objectStartBitMap[mapIndex] |= (1 << (objectStartNumber & 7));
    headerAddress += header->size();
    ASSERT(headerAddress <= payloadEnd());
  }
  m_objectStartBitMapComputed = true;
}
