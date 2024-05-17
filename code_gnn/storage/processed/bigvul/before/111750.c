void AddOsStrings(unsigned bitmask, ListValue* list) {
  struct {
    unsigned bit;
    const char* const name;
  } kBitsToOs[] = {
    {kOsMac, "Mac"},
    {kOsWin, "Windows"},
    {kOsLinux, "Linux"},
    {kOsCrOS, "Chrome OS"},
  };
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(kBitsToOs); ++i)
    if (bitmask & kBitsToOs[i].bit)
      list->Append(new StringValue(kBitsToOs[i].name));
}