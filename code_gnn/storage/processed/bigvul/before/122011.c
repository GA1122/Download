bool GetClientStateValue(InstallationLevel level,
                         const wchar_t* app_guid,
                         const wchar_t* value_name,
                         string16* value) {
  HKEY root_key = (level == USER_LEVEL_INSTALLATION) ?
      HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE;
  string16 subkey(kGoogleRegClientStateKey);
  subkey.append(1, L'\\').append(app_guid);
  base::win::RegKey reg_key;
  if (reg_key.Open(root_key, subkey.c_str(),
                   KEY_QUERY_VALUE | KEY_WOW64_32KEY) == ERROR_SUCCESS) {
    if (reg_key.ReadValue(value_name, value) == ERROR_SUCCESS) {
      return true;
    }
  }
  return false;
}
