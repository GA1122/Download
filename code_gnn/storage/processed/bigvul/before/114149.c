DWORD SetTokenIntegrityLevel(HANDLE token, IntegrityLevel integrity_level) {
  if (base::win::GetVersion() < base::win::VERSION_VISTA)
    return ERROR_SUCCESS;

  const wchar_t* integrity_level_str = GetIntegrityLevelString(integrity_level);
  if (!integrity_level_str) {
    return ERROR_SUCCESS;
  }

  PSID integrity_sid = NULL;
  if (!::ConvertStringSidToSid(integrity_level_str, &integrity_sid))
    return ::GetLastError();

  TOKEN_MANDATORY_LABEL label = {0};
  label.Label.Attributes = SE_GROUP_INTEGRITY;
  label.Label.Sid = integrity_sid;

  DWORD size = sizeof(TOKEN_MANDATORY_LABEL) + ::GetLengthSid(integrity_sid);
  BOOL result = ::SetTokenInformation(token, TokenIntegrityLevel, &label,
                                      size);
  ::LocalFree(integrity_sid);

  return result ? ERROR_SUCCESS : ::GetLastError();
}
