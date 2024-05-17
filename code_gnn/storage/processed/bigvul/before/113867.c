bool SaveFileAs(HWND owner,
                const std::wstring& suggested_name,
                std::wstring* final_name) {
  std::wstring file_ext = FilePath(suggested_name).Extension().insert(0, L"*");
  std::wstring filter = FormatFilterForExtensions(
      std::vector<std::wstring>(1, file_ext),
      std::vector<std::wstring>(),
      true);
  unsigned index = 1;
  return SaveFileAsWithFilter(owner,
                              suggested_name,
                              filter,
                              L"",
                              false,
                              &index,
                              final_name);
}
