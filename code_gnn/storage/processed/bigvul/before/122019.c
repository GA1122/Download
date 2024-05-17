bool IsProductInstalled(InstallationLevel level, const wchar_t* app_guid) {
  HKEY root_key = (level == USER_LEVEL_INSTALLATION) ?
      HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE;
  string16 subkey(kGoogleRegClientsKey);
  subkey.append(1, L'\\').append(app_guid);
  base::win::RegKey reg_key;
  return reg_key.Open(root_key, subkey.c_str(),
                      KEY_QUERY_VALUE | KEY_WOW64_32KEY) == ERROR_SUCCESS &&
      reg_key.HasValue(kRegVersionField);
}
