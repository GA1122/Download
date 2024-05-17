uint32_t GenerateNextFlushId() {
  return static_cast<uint32_t>(g_flush_id.GetNext());
}
