DWORD SetDWValue(HKEY* key, const wchar_t* property, DWORD value) {
  return ::RegSetValueEx(*key,
                         property,
                         0,
                         REG_DWORD,
                         reinterpret_cast<LPBYTE>(&value),
                         sizeof(value));
}
