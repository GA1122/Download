int SBEntry::Size(Type type, int prefix_count) {
  return sizeof(Data) + prefix_count * PrefixSize(type);
}
