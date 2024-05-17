bool RecursiveDirectoryCreate(const std::wstring& full_path) {
  const wchar_t* full_path_str = full_path.c_str();
  DWORD file_attributes = ::GetFileAttributes(full_path_str);
  if (file_attributes != INVALID_FILE_ATTRIBUTES) {
    if ((file_attributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
      Trace(L"%hs( %ls directory exists )\n", __func__, full_path_str);
      return true;
    }
    Trace(L"%hs( %ls directory conflicts with an existing file. )\n", __func__,
          full_path_str);
    return false;
  }


  std::wstring parent_path;
  std::size_t pos = full_path.find_last_of(L"/\\");
  if (pos != std::wstring::npos) {
    parent_path = full_path.substr(0, pos);
    if (!RecursiveDirectoryCreate(parent_path)) {
      Trace(L"Failed to create one of the parent directories");
      return false;
    }
  }
  if (!::CreateDirectory(full_path_str, nullptr)) {
    DWORD error_code = ::GetLastError();
    if (error_code == ERROR_ALREADY_EXISTS && DirectoryExists(full_path_str)) {
      return true;
    } else {
      Trace(L"Failed to create directory %ls, last error is %d\n",
            full_path_str, error_code);
      return false;
    }
  }
  return true;
}
