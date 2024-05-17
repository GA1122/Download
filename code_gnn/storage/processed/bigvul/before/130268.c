static LPITEMIDLIST GetPidlFromPath(const base::FilePath& path) {
  LPITEMIDLIST pidl = NULL;
  LPSHELLFOLDER desktop_folder = NULL;
  LPWSTR path_str = const_cast<LPWSTR>(path.value().c_str());

  if (FAILED(SHGetDesktopFolder(&desktop_folder)))
    return NULL;

  HRESULT hr = desktop_folder->ParseDisplayName(
      NULL, NULL, path_str, NULL, &pidl, NULL);

  return SUCCEEDED(hr) ? pidl : NULL;
}
