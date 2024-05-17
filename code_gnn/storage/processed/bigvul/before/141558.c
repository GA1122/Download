bool IsCurrentSessionRemote() {
  static const wchar_t kRdpSettingsKeyName[] =
      L"SYSTEM\\CurrentControlSet\\Control\\Terminal Server";
  static const wchar_t kGlassSessionIdValueName[] = L"GlassSessionId";

  if (::GetSystemMetrics(SM_REMOTESESSION))
    return true;

  DWORD glass_session_id = 0;
  DWORD current_session_id = 0;
  base::win::RegKey key(HKEY_LOCAL_MACHINE, kRdpSettingsKeyName, KEY_READ);
  if (!::ProcessIdToSessionId(::GetCurrentProcessId(), &current_session_id) ||
      !key.Valid() ||
      key.ReadValueDW(kGlassSessionIdValueName, &glass_session_id) !=
          ERROR_SUCCESS) {
    return false;
  }

  return current_session_id != glass_session_id;
}
