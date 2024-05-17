int HttpUtil::LocateStartOfStatusLine(const char* buf, int buf_len) {
  const int slop = 4;
  const int http_len = 4;

  if (buf_len >= http_len) {
    int i_max = std::min(buf_len - http_len, slop);
    for (int i = 0; i <= i_max; ++i) {
      if (LowerCaseEqualsASCII(buf + i, buf + i + http_len, "http"))
        return i;
    }
  }
  return -1;   
}
