bool HttpUtil::IsNonCoalescingHeader(string::const_iterator name_begin,
                                     string::const_iterator name_end) {
  const char* kNonCoalescingHeaders[] = {
    "date",
    "expires",
    "last-modified",
    "location",   
    "retry-after",
    "set-cookie",
    "www-authenticate",
    "proxy-authenticate"
  };
  for (size_t i = 0; i < arraysize(kNonCoalescingHeaders); ++i) {
    if (LowerCaseEqualsASCII(name_begin, name_end, kNonCoalescingHeaders[i]))
      return true;
  }
  return false;
}
