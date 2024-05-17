string16 BuildString16(const wchar_t* s) {
#if defined(WCHAR_T_IS_UTF16)
  return string16(s);
#elif defined(WCHAR_T_IS_UTF32)
  string16 u16;
  while (*s != 0) {
    DCHECK_LE(static_cast<unsigned int>(*s), 0xFFFFu);
    u16.push_back(*s++);
  }
  return u16;
#endif
}
