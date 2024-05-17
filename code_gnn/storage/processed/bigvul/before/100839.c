static bool IsLineSegmentContinuable(const char* begin, const char* end) {
  if (begin == end)
    return false;

  const char* colon = std::find(begin, end, ':');
  if (colon == end)
    return false;

  const char* name_begin = begin;
  const char* name_end = colon;

  if (name_begin == name_end)
    return false;

  if (HttpUtil::IsLWS(*name_begin))
    return false;

  return true;
}
