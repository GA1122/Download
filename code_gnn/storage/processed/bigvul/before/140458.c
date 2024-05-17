  const char* GetAlternateProtocolFromParam() {
    return
        AlternateProtocolToString(AlternateProtocolFromNextProto(GetParam()));
  }
