void GCInfoTable::resize() {
  static const int gcInfoZapValue = 0x33;
  const size_t initialSize = 512;

  size_t newSize = s_gcInfoTableSize ? 2 * s_gcInfoTableSize : initialSize;
  ASSERT(newSize < GCInfoTable::maxIndex);
  s_gcInfoTable = reinterpret_cast<GCInfo const**>(WTF::Partitions::fastRealloc(
      s_gcInfoTable, newSize * sizeof(GCInfo), "GCInfo"));
  ASSERT(s_gcInfoTable);
  memset(reinterpret_cast<uint8_t*>(s_gcInfoTable) +
             s_gcInfoTableSize * sizeof(GCInfo),
         gcInfoZapValue, (newSize - s_gcInfoTableSize) * sizeof(GCInfo));
  s_gcInfoTableSize = newSize;
}
