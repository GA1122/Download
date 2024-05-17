bool SelectFileDialogImpl::RunSelectFolderDialog(const std::wstring& title,
                                                 HWND owner,
                                                 FilePath* path) {
  DCHECK(path);

  wchar_t dir_buffer[MAX_PATH + 1];

  bool result = false;
  BROWSEINFO browse_info = {0};
  browse_info.hwndOwner = owner;
  browse_info.lpszTitle = title.c_str();
  browse_info.pszDisplayName = dir_buffer;
  browse_info.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;

  if (path->value().length()) {
    browse_info.lParam = (LPARAM)path->value().c_str();
    browse_info.lpfn = &BrowseCallbackProc;
  }

  LPITEMIDLIST list = SHBrowseForFolder(&browse_info);
  DisableOwner(owner);
  if (list) {
    STRRET out_dir_buffer;
    ZeroMemory(&out_dir_buffer, sizeof(out_dir_buffer));
    out_dir_buffer.uType = STRRET_WSTR;
    base::win::ScopedComPtr<IShellFolder> shell_folder;
    if (SHGetDesktopFolder(shell_folder.Receive()) == NOERROR) {
      HRESULT hr = shell_folder->GetDisplayNameOf(list, SHGDN_FORPARSING,
                                                  &out_dir_buffer);
      if (SUCCEEDED(hr) && out_dir_buffer.uType == STRRET_WSTR) {
        *path = FilePath(out_dir_buffer.pOleStr);
        CoTaskMemFree(out_dir_buffer.pOleStr);
        result = true;
      } else {
        wchar_t old_out_dir_buffer[MAX_PATH + 1];
        if (SHGetPathFromIDList(list, old_out_dir_buffer)) {
          *path = FilePath(old_out_dir_buffer);
          result = true;
        }
      }

      file_util::ResolveShortcut(path);
    }
    CoTaskMemFree(list);
  }
  return result;
}
