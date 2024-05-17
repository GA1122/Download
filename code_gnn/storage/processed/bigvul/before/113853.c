std::wstring AppendExtensionIfNeeded(const std::wstring& filename,
                                     const std::wstring& filter_selected,
                                     const std::wstring& suggested_ext) {
  DCHECK(!filename.empty());
  std::wstring return_value = filename;

  std::wstring file_extension(
      GetExtensionWithoutLeadingDot(FilePath(filename).Extension()));
  std::wstring key(L"." + file_extension);
  if (!(filter_selected.empty() || filter_selected == L"*.*") &&
      !base::win::RegKey(HKEY_CLASSES_ROOT, key.c_str(), KEY_READ).Valid() &&
      file_extension != suggested_ext) {
    if (return_value[return_value.length() - 1] != L'.')
      return_value.append(L".");
    return_value.append(suggested_ext);
  }

  size_t index = return_value.find_last_not_of(L'.');
  if (index < return_value.size() - 1)
    return_value.resize(index + 1);

  return return_value;
}
