bool CopyProcessToken(DWORD desired_access, ScopedHandle* token_out) {
  ScopedHandle process_token;
  if (!OpenProcessToken(GetCurrentProcess(),
                        TOKEN_DUPLICATE | desired_access,
                        process_token.Receive())) {
    LOG_GETLASTERROR(ERROR) << "Failed to open process token";
    return false;
  }

  ScopedHandle copied_token;
  if (!DuplicateTokenEx(process_token,
                        desired_access,
                        NULL,
                        SecurityImpersonation,
                        TokenPrimary,
                        copied_token.Receive())) {
    LOG_GETLASTERROR(ERROR) << "Failed to duplicate the process token";
    return false;
  }

  *token_out = copied_token.Pass();
  return true;
}
