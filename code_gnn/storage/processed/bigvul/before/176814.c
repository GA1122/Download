static size_t consumeDigits(const char16_t* start, const char16_t* end) {
 const char16_t* c = start;
 for (; c != end && *c >= u'0' && *c <= u'9'; c++) {}
 return static_cast<size_t>(c - start);
}
