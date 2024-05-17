bool IsFirefoxDefaultBrowser() {
  base::string16 app_cmd;
  base::win::RegKey key(HKEY_CURRENT_USER, ShellUtil::kRegVistaUrlPrefs,
                        KEY_READ);
  return key.Valid() && key.ReadValue(L"Progid", &app_cmd) == ERROR_SUCCESS &&
         app_cmd == L"FirefoxURL";
}
