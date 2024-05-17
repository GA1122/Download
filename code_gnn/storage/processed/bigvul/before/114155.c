int TestOpenKey(HKEY base_key, std::wstring subkey) {
  HKEY key;
  LONG err_code = ::RegOpenKeyEx(base_key,
                                 subkey.c_str(),
                                 0,   
                                 MAXIMUM_ALLOWED,
                                 &key);
  if (ERROR_SUCCESS == err_code) {
    ::RegCloseKey(key);
    return SBOX_TEST_SUCCEEDED;
  } else if (ERROR_INVALID_HANDLE == err_code ||
             ERROR_ACCESS_DENIED  == err_code) {
    return SBOX_TEST_DENIED;
  } else {
    return SBOX_TEST_FAILED_TO_EXECUTE_COMMAND;
  }
}
