int SBEntry::PrefixSize(Type type) {
  switch (type) {
    case ADD_PREFIX:
      return sizeof(SBPrefix);
    case ADD_FULL_HASH:
      return sizeof(SBFullHash);
    case SUB_PREFIX:
      return sizeof(SBSubPrefix);
    case SUB_FULL_HASH:
      return sizeof(SBSubFullHash);
    default:
      NOTREACHED();
      return 0;
  }
}
