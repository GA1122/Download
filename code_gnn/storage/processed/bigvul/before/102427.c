bool IsShellIntegratedExtension(const string16& extension) {
  string16 extension_lower = StringToLowerASCII(extension);

  static const wchar_t* const integrated_extensions[] = {
    L"local",
    L"lnk",
  };

  for (int i = 0; i < arraysize(integrated_extensions); ++i) {
    if (extension_lower == integrated_extensions[i])
      return true;
  }

  if (!extension_lower.empty() && extension_lower[0] == L'{' &&
      extension_lower[extension_lower.length() - 1] == L'}')
    return true;

  return false;
}
