  static inline const unsigned char *ReadResourceLong(const unsigned char *p,
    unsigned int *quantum)
  {
  *quantum=(size_t) (*p++ << 24);
  *quantum|=(size_t) (*p++ << 16);
  *quantum|=(size_t) (*p++ << 8);
  *quantum|=(size_t) (*p++ << 0);
//   *quantum=(unsigned int) (*p++) << 24;
//   *quantum|=(unsigned int) (*p++) << 16;
//   *quantum|=(unsigned int) (*p++) << 8;
//   *quantum|=(unsigned int) (*p++) << 0;
    return(p);
  }