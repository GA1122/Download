DWORD CreateRestrictedToken(HANDLE *token_handle,
                            TokenLevel security_level,
                            IntegrityLevel integrity_level,
                            TokenType token_type) {
  if (!token_handle)
    return ERROR_BAD_ARGUMENTS;

  RestrictedToken restricted_token;
  restricted_token.Init(NULL);   

  std::vector<std::wstring> privilege_exceptions;
  std::vector<Sid> sid_exceptions;

  bool deny_sids = true;
  bool remove_privileges = true;

  switch (security_level) {
    case USER_UNPROTECTED: {
      deny_sids = false;
      remove_privileges = false;
      break;
    }
    case USER_RESTRICTED_SAME_ACCESS: {
      deny_sids = false;
      remove_privileges = false;

      unsigned err_code = restricted_token.AddRestrictingSidAllSids();
      if (ERROR_SUCCESS != err_code)
        return err_code;

      break;
    }
    case USER_NON_ADMIN: {
      sid_exceptions.push_back(WinBuiltinUsersSid);
      sid_exceptions.push_back(WinWorldSid);
      sid_exceptions.push_back(WinInteractiveSid);
      sid_exceptions.push_back(WinAuthenticatedUserSid);
      privilege_exceptions.push_back(SE_CHANGE_NOTIFY_NAME);
      break;
    }
    case USER_INTERACTIVE: {
      sid_exceptions.push_back(WinBuiltinUsersSid);
      sid_exceptions.push_back(WinWorldSid);
      sid_exceptions.push_back(WinInteractiveSid);
      sid_exceptions.push_back(WinAuthenticatedUserSid);
      privilege_exceptions.push_back(SE_CHANGE_NOTIFY_NAME);
      restricted_token.AddRestrictingSid(WinBuiltinUsersSid);
      restricted_token.AddRestrictingSid(WinWorldSid);
      restricted_token.AddRestrictingSid(WinRestrictedCodeSid);
      restricted_token.AddRestrictingSidCurrentUser();
      restricted_token.AddRestrictingSidLogonSession();
      break;
    }
    case USER_LIMITED: {
      sid_exceptions.push_back(WinBuiltinUsersSid);
      sid_exceptions.push_back(WinWorldSid);
      sid_exceptions.push_back(WinInteractiveSid);
      privilege_exceptions.push_back(SE_CHANGE_NOTIFY_NAME);
      restricted_token.AddRestrictingSid(WinBuiltinUsersSid);
      restricted_token.AddRestrictingSid(WinWorldSid);
      restricted_token.AddRestrictingSid(WinRestrictedCodeSid);

      if (base::win::GetVersion() >= base::win::VERSION_VISTA)
        restricted_token.AddRestrictingSidLogonSession();
      break;
    }
    case USER_RESTRICTED: {
      privilege_exceptions.push_back(SE_CHANGE_NOTIFY_NAME);
      restricted_token.AddUserSidForDenyOnly();
      restricted_token.AddRestrictingSid(WinRestrictedCodeSid);
      break;
    }
    case USER_LOCKDOWN: {
      restricted_token.AddUserSidForDenyOnly();
      restricted_token.AddRestrictingSid(WinNullSid);
      break;
    }
    default: {
      return ERROR_BAD_ARGUMENTS;
    }
  }

  DWORD err_code = ERROR_SUCCESS;
  if (deny_sids) {
    err_code = restricted_token.AddAllSidsForDenyOnly(&sid_exceptions);
    if (ERROR_SUCCESS != err_code)
      return err_code;
  }

  if (remove_privileges) {
    err_code = restricted_token.DeleteAllPrivileges(&privilege_exceptions);
    if (ERROR_SUCCESS != err_code)
      return err_code;
  }

  restricted_token.SetIntegrityLevel(integrity_level);

  switch (token_type) {
    case PRIMARY: {
      err_code = restricted_token.GetRestrictedTokenHandle(token_handle);
      break;
    }
    case IMPERSONATION: {
      err_code = restricted_token.GetRestrictedTokenHandleForImpersonation(
          token_handle);
      break;
    }
    default: {
      err_code = ERROR_BAD_ARGUMENTS;
      break;
    }
  }

  return err_code;
}
