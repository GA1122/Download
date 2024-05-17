SBEntry* SBEntry::Enlarge(int extra_prefixes) {
  int new_prefix_count = prefix_count() + extra_prefixes;
  SBEntry* rv = SBEntry::Create(type(), new_prefix_count);
  memcpy(rv, this, Size());   
  rv->set_prefix_count(new_prefix_count);
  Destroy();
  return rv;
}
