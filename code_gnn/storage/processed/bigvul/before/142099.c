HRESULT EnrollToGoogleMdmIfNeeded(const base::DictionaryValue& properties) {
  USES_CONVERSION;
  LOGFN(INFO);

  base::string16 email = GetDictString(&properties, kKeyEmail);
  base::string16 token = GetDictString(&properties, kKeyMdmIdToken);

  if (email.empty()) {
    LOGFN(ERROR) << "Email is empty";
    return E_INVALIDARG;
  }

  if (token.empty()) {
    LOGFN(ERROR) << "MDM id token is empty";
    return E_INVALIDARG;
  }

  base::string16 mdm_url = GetMdmUrl();
  if (mdm_url.empty())
    return S_OK;

  LOGFN(INFO) << "MDM_URL=" << mdm_url
              << " token=" << base::string16(token.c_str(), 10);

  base::string16 serial_number =
      base::win::WmiComputerSystemInfo::Get().serial_number();

  if (serial_number.empty()) {
    LOGFN(ERROR) << "Failed to get serial number.";
    return -1;
  }

  base::DictionaryValue registration_data;
  registration_data.SetString("serial_number", serial_number);
  registration_data.SetString("id_token", token);
  std::string registration_data_str;
  if (!base::JSONWriter::Write(registration_data, &registration_data_str)) {
    LOGFN(ERROR) << "JSONWriter::Write(registration_data)";
    return E_FAIL;
  }

  HRESULT hr =
      RegisterWithGoogleDeviceManagement(mdm_url, email, registration_data_str);
  if (FAILED(hr))
    LOGFN(ERROR) << "RegisterWithGoogleDeviceManagement hr=" << putHR(hr);

  return hr;
}
