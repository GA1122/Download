static const char* FindFirstNonLWS(const char* begin, const char* end) {
  for (const char* cur = begin; cur != end; ++cur) {
    if (!HttpUtil::IsLWS(*cur))
      return cur;
  }
  return end;   
}
