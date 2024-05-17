bool CreatePrivilegedToken(ScopedHandle* token_out) {
  ScopedHandle privileged_token;
  DWORD desired_access = TOKEN_ADJUST_PRIVILEGES | TOKEN_IMPERSONATE |
                         TOKEN_DUPLICATE | TOKEN_QUERY;
  if (!CopyProcessToken(desired_access, &privileged_token)) {
    return false;
  }

  TOKEN_PRIVILEGES state;
  state.PrivilegeCount = 1;
  state.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
  if (!LookupPrivilegeValue(NULL, SE_TCB_NAME, &state.Privileges[0].Luid)) {
    LOG_GETLASTERROR(ERROR) <<
        "Failed to lookup the LUID for the SE_TCB_NAME privilege";
    return false;
  }

  if (!AdjustTokenPrivileges(privileged_token, FALSE, &state, 0, NULL, 0)) {
    LOG_GETLASTERROR(ERROR) <<
        "Failed to enable SE_TCB_NAME privilege in a token";
    return false;
  }

  *token_out = privileged_token.Pass();
  return true;
}
