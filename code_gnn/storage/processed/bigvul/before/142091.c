HRESULT ValidateResult(const base::DictionaryValue* result, BSTR* status_text) {
  DCHECK(result);
  DCHECK(status_text);

  const base::Value* exit_code_value =
      result->FindKeyOfType(kKeyExitCode, base::Value::Type::INTEGER);
  int exit_code = exit_code_value->GetInt();
  if (exit_code != kUiecSuccess) {
    switch (exit_code) {
      case kUiecAbort:
        return E_ABORT;
      case kUiecTimeout:
      case kUiecKilled:
        NOTREACHED() << "Internal codes, not returned by GLS";
        break;
      case kUiecEMailMissmatch:
        *status_text =
            CGaiaCredentialBase::AllocErrorString(IDS_EMAIL_MISMATCH_BASE);
        break;
      case kUiecInvalidEmailDomain:
        *status_text = CGaiaCredentialBase::AllocErrorString(
            IDS_INVALID_EMAIL_DOMAIN_BASE);
        break;
      case kUiecMissingSigninData:
        *status_text =
            CGaiaCredentialBase::AllocErrorString(IDS_INVALID_UI_RESPONSE_BASE);
        break;
    }
    return E_FAIL;
  }


  bool has_error = false;
  std::string email = GetDictStringUTF8(result, kKeyEmail);
  if (email.empty()) {
    LOGFN(ERROR) << "Email is empty";
    has_error = true;
  }

  std::string fullname = GetDictStringUTF8(result, kKeyFullname);
  if (fullname.empty()) {
    LOGFN(ERROR) << "Full name is empty";
    has_error = true;
  }

  std::string id = GetDictStringUTF8(result, kKeyId);
  if (id.empty()) {
    LOGFN(ERROR) << "Id is empty";
    has_error = true;
  }

  std::string mdm_id_token = GetDictStringUTF8(result, kKeyMdmIdToken);
  if (mdm_id_token.empty()) {
    LOGFN(ERROR) << "mdm id token is empty";
    has_error = true;
  }

  std::string password = GetDictStringUTF8(result, kKeyPassword);
  if (password.empty()) {
    LOGFN(ERROR) << "Password is empty";
    has_error = true;
  }

  std::string refresh_token = GetDictStringUTF8(result, kKeyRefreshToken);
  if (refresh_token.empty()) {
    LOGFN(ERROR) << "refresh token is empty";
    has_error = true;
  }

  std::string token_handle = GetDictStringUTF8(result, kKeyTokenHandle);
  if (token_handle.empty()) {
    LOGFN(ERROR) << "Token handle is empty";
    has_error = true;
  }

  if (has_error) {
    *status_text =
        CGaiaCredentialBase::AllocErrorString(IDS_INVALID_UI_RESPONSE_BASE);
    return E_UNEXPECTED;
  }

  return S_OK;
}
