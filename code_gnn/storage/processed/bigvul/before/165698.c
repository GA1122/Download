std::string UTF16ToUTF8(const std::wstring& source) {
  if (source.empty() ||
      static_cast<int>(source.size()) > std::numeric_limits<int>::max()) {
    return std::string();
  }
  int size = ::WideCharToMultiByte(CP_UTF8, 0, &source[0],
                                   static_cast<int>(source.size()), nullptr, 0,
                                   nullptr, nullptr);
  std::string result(size, '\0');
  if (::WideCharToMultiByte(CP_UTF8, 0, &source[0],
                            static_cast<int>(source.size()), &result[0], size,
                            nullptr, nullptr) != size) {
    assert(false);
    return std::string();
  }
  return result;
}
