void SplitString(const std::wstring& str,
                 wchar_t c,
                 std::vector<std::wstring>* result) {
  std::vector<string16> result16;
  SplitString(WideToUTF16(str), c, &result16);
  for (size_t i = 0; i < result16.size(); ++i)
    result->push_back(UTF16ToWide(result16[i]));
}
