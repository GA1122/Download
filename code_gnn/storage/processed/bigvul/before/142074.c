HRESULT CGaiaCredentialBase::OnUserAuthenticated(BSTR authentication_info,
                                                 BSTR* status_text) {
  USES_CONVERSION;
  DCHECK(status_text);

  logon_ui_process_ = INVALID_HANDLE_VALUE;

  std::string json_string;
  base::UTF16ToUTF8(OLE2CW(authentication_info),
                    ::SysStringLen(authentication_info), &json_string);
  std::unique_ptr<base::Value> properties = base::JSONReader::ReadDeprecated(
      json_string, base::JSON_ALLOW_TRAILING_COMMAS);
  if (!properties || !properties->is_dict()) {
    LOGFN(ERROR) << "base::JSONReader::Read failed to translate to JSON";
    *status_text = AllocErrorString(IDS_INVALID_UI_RESPONSE_BASE);
    return E_FAIL;
  }

  std::unique_ptr<base::DictionaryValue> dict =
      base::DictionaryValue::From(std::move(properties));

  HRESULT hr = ValidateResult(dict.get(), status_text);
  if (FAILED(hr)) {
    LOGFN(ERROR) << "ValidateResult hr=" << putHR(hr);
    return hr;
  }

  hr = ValidateOrCreateUser(dict.get(), &domain_, &username_, &user_sid_,
                            status_text);
  if (FAILED(hr)) {
    LOGFN(ERROR) << "ValidateOrCreateUser hr=" << putHR(hr);
    return hr;
  }

  authentication_results_ = std::move(dict);

  password_ = ::SysAllocString(
      GetDictString(authentication_results_, kKeyPassword).c_str());

  if (events_)
    events_->SetFieldInteractiveState(this, FID_SUBMIT, CPFIS_DISABLED);

  if (!AreCredentialsValid()) {
    DisplayPasswordField(IDS_PASSWORD_UPDATE_NEEDED_BASE);
    return S_FALSE;
  }

  result_status_ = STATUS_SUCCESS;

  return provider_->OnUserAuthenticated(static_cast<IGaiaCredential*>(this),
                                        username_, password_, user_sid_, TRUE);
}
