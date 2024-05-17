void HttpUtil::TrimLWS(string::const_iterator* begin,
                       string::const_iterator* end) {
  while (*begin < *end && IsLWS((*begin)[0]))
    ++(*begin);

  while (*begin < *end && IsLWS((*end)[-1]))
    --(*end);
}
