static const char* FindStatusLineEnd(const char* begin, const char* end) {
  size_t i = base::StringPiece(begin, end - begin).find_first_of("\r\n");
  if (i == base::StringPiece::npos)
    return end;
  return begin + i;
}
