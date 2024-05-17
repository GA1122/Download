void AddDllsFromRegistryToBlacklist() {
  HKEY key = NULL;
  LONG result = ::RegOpenKeyEx(HKEY_CURRENT_USER,
                               kRegistryFinchListPath,
                               0,
                               KEY_QUERY_VALUE | KEY_SET_VALUE,
                               &key);

  if (result != ERROR_SUCCESS)
    return;

  DWORD value_len;
  DWORD name_len = MAX_PATH;
  std::vector<wchar_t> name_buffer(name_len);
  for (int i = 0; result == ERROR_SUCCESS; ++i) {
    name_len = MAX_PATH;
    value_len = 0;
    result = ::RegEnumValue(
        key, i, &name_buffer[0], &name_len, NULL, NULL, NULL, &value_len);
    if (result != ERROR_SUCCESS)
      break;

    name_len = name_len + 1;
    value_len = value_len + 1;
    std::vector<wchar_t> value_buffer(value_len);
    result = ::RegEnumValue(key, i, &name_buffer[0], &name_len, NULL, NULL,
                            reinterpret_cast<BYTE*>(&value_buffer[0]),
                            &value_len);
    if (result != ERROR_SUCCESS)
      break;
    value_buffer[value_len - 1] = L'\0';
    AddDllToBlacklist(&value_buffer[0]);
  }

  ::RegCloseKey(key);
  return;
}
