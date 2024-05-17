LaunchMode GetLaunchShortcutKind() {
  STARTUPINFOW si = { sizeof(si) };
  GetStartupInfoW(&si);
  if (si.dwFlags & 0x800) {
    if (!si.lpTitle)
      return LM_SHORTCUT_NONAME;
    std::wstring shortcut(si.lpTitle);
    if (shortcut.find(L"\\Quick Launch\\") != std::wstring::npos)
      return LM_SHORTCUT_QUICKLAUNCH;
    std::wstring appdata_path = base::SysInfo::GetEnvVar(L"USERPROFILE");
    if (!appdata_path.empty() &&
        shortcut.find(appdata_path) != std::wstring::npos)
      return LM_SHORTCUT_DESKTOP;
    return LM_SHORTCUT_UNKNOWN;
  }
  return LM_SHORTCUT_NONE;
}