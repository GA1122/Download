bool GetLanguageAndCodePageFromVersionResource(const char* version_resource,
                                               WORD* language,
                                               WORD* code_page) {
  if (!version_resource)
    return false;

  struct LanguageAndCodePage {
    WORD language;
    WORD code_page;
  };

  LanguageAndCodePage* translation_info = nullptr;
  uint32_t data_size_in_bytes = 0;
  BOOL query_result = VerQueryValueW(
      version_resource, L"\\VarFileInfo\\Translation",
      reinterpret_cast<void**>(&translation_info), &data_size_in_bytes);
  if (!query_result)
    return false;

  *language = translation_info->language;
  *code_page = translation_info->code_page;
  return true;
}
