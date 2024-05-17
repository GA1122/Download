 std::wstring MakePathToSys32(const wchar_t* name, bool is_obj_man_path) {
   wchar_t windows_path[MAX_PATH] = {0};
   if (0 == ::GetSystemWindowsDirectoryW(windows_path, MAX_PATH))
    return std::wstring();

  std::wstring full_path(windows_path);
  if (full_path.empty())
    return full_path;

  if (is_obj_man_path)
    full_path.insert(0, L"\\??\\");

  full_path += L"\\system32\\";
  full_path += name;
   return full_path;
 }
