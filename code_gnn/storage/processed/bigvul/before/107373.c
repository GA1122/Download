int FindInPageWchar(TabContentsWrapper* tab,
                    const wchar_t* search_str,
                    bool forward,
                    bool case_sensitive,
                    int* ordinal) {
  return ui_test_utils::FindInPage(
      tab, WideToUTF16(std::wstring(search_str)),
      forward, case_sensitive, ordinal);
}
