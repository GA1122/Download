bool SaveFileAsWithFilter(HWND owner,
                          const std::wstring& suggested_name,
                          const std::wstring& filter,
                          const std::wstring& def_ext,
                          bool ignore_suggested_ext,
                          unsigned* index,
                          std::wstring* final_name) {
  DCHECK(final_name);
  DCHECK(!filter.empty());
  const FilePath suggested_path(suggested_name);
  std::wstring file_part = suggested_path.BaseName().value();
  if (file_part.size() == 1 && file_part[0] == L'\\')
    file_part.clear();

  wchar_t file_name[MAX_PATH];
  base::wcslcpy(file_name, file_part.c_str(), arraysize(file_name));

  OPENFILENAME save_as;
  ZeroMemory(&save_as, sizeof(save_as));
  save_as.lStructSize = sizeof(OPENFILENAME);
  save_as.hwndOwner = owner;
  save_as.hInstance = NULL;

  save_as.lpstrFilter = filter.empty() ? NULL : filter.c_str();

  save_as.lpstrCustomFilter = NULL;
  save_as.nMaxCustFilter = 0;
  save_as.nFilterIndex = *index;
  save_as.lpstrFile = file_name;
  save_as.nMaxFile = arraysize(file_name);
  save_as.lpstrFileTitle = NULL;
  save_as.nMaxFileTitle = 0;

  std::wstring directory;
  if (!suggested_name.empty())
     directory = suggested_path.DirName().value();

  save_as.lpstrInitialDir = directory.c_str();
  save_as.lpstrTitle = NULL;
  save_as.Flags = OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING |
                  OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST;
  save_as.lpstrDefExt = &def_ext[0];
  save_as.lCustData = NULL;

  if (base::win::GetVersion() < base::win::VERSION_VISTA) {
    save_as.Flags |= OFN_ENABLEHOOK;
    save_as.lpfnHook = &SaveAsDialogHook;
  }

  save_as.pvReserved = NULL;
  save_as.dwReserved = 0;

  if (!GetSaveFileName(&save_as)) {
    DWORD error_code = CommDlgExtendedError();
    if (error_code != 0) {
      NOTREACHED() << "GetSaveFileName failed with code: " << error_code;
    }
    return false;
  }

  final_name->assign(save_as.lpstrFile);
  *index = save_as.nFilterIndex;

  std::vector<std::wstring> filters;
  if (!filter.empty() && save_as.nFilterIndex > 0)
    base::SplitString(filter, '\0', &filters);
  std::wstring filter_selected;
  if (!filters.empty())
    filter_selected = filters[(2 * (save_as.nFilterIndex - 1)) + 1];

  std::wstring suggested_ext;
  if (!ignore_suggested_ext)
    suggested_ext = GetExtensionWithoutLeadingDot(suggested_path.Extension());

  if (suggested_ext.empty())
    suggested_ext = def_ext;

  *final_name =
      AppendExtensionIfNeeded(*final_name, filter_selected, suggested_ext);
  return true;
}
