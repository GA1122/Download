HRESULT CGaiaCredentialBase::GetGlsCommandline(
    base::CommandLine* command_line) {
  DCHECK(command_line);
  HRESULT hr = GetBaseGlsCommandline(command_line);
  if (FAILED(hr)) {
    LOGFN(ERROR) << "GetBaseGlsCommandline hr=" << putHR(hr);
    return hr;
  }

  base::string16 email_domains = GetEmailDomains();
  if (email_domains[0])
    command_line->AppendSwitchNative(kEmailDomainsSwitch, email_domains);

  hr = GetUserGlsCommandline(command_line);
  if (FAILED(hr)) {
    LOGFN(ERROR) << "GetBaseGlsCommandline hr=" << putHR(hr);
    return hr;
  }

  LOGFN(INFO) << "Command line: " << command_line->GetCommandLineString();
  return S_OK;
}
