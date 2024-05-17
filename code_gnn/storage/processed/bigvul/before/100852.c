std::string HttpUtil::Unquote(const std::string& str) {
  return Unquote(str.begin(), str.end());
}
