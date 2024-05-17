bool LeaveSetupBeacon() {
  HKEY key = NULL;
  DWORD disposition = 0;
  LONG result = ::RegCreateKeyEx(HKEY_CURRENT_USER,
                                 kRegistryBeaconPath,
                                 0,
                                 NULL,
                                 REG_OPTION_NON_VOLATILE,
                                 KEY_QUERY_VALUE | KEY_SET_VALUE,
                                 NULL,
                                 &key,
                                 &disposition);
  if (result != ERROR_SUCCESS)
    return false;

  DWORD blacklist_state = BLACKLIST_STATE_MAX;
  DWORD blacklist_state_size = sizeof(blacklist_state);
  DWORD type = 0;
  result = ::RegQueryValueEx(key,
                             kBeaconState,
                             0,
                             &type,
                             reinterpret_cast<LPBYTE>(&blacklist_state),
                             &blacklist_state_size);

  if (result != ERROR_SUCCESS || blacklist_state == BLACKLIST_DISABLED ||
      type != REG_DWORD) {
    ::RegCloseKey(key);
    return false;
  }

  if (!GenerateStateFromBeaconAndAttemptCount(&key, blacklist_state)) {
    ::RegCloseKey(key);
    return false;
  }

  result = SetDWValue(&key, kBeaconState, BLACKLIST_SETUP_RUNNING);
  ::RegCloseKey(key);

  return (result == ERROR_SUCCESS);
}