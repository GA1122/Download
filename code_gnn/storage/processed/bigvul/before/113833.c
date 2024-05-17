bool IsExpandedModuleName(HMODULE module, const wchar_t* module_name) {
  wchar_t path[MAX_PATH];
  DWORD sz = ::GetModuleFileNameW(module, path, arraysize(path));
  if ((sz == arraysize(path)) || (sz == 0)) {
    return false;
  }
  if (!::GetLongPathName(path, path, arraysize(path)))
    return false;
  FilePath fname(path);
  return (fname.BaseName().value() == module_name);
}
