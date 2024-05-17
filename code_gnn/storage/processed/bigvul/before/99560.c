SBEntry* SBEntry::Create(Type type, int prefix_count) {
  int size = Size(type, prefix_count);
  SBEntry *rv = static_cast<SBEntry*>(malloc(size));
  memset(rv, 0, size);
  rv->set_type(type);
  rv->set_prefix_count(prefix_count);
  return rv;
}
