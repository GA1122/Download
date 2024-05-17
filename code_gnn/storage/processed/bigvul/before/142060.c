base::string16 GetEmailDomains() {
  std::vector<wchar_t> email_domains(16);
  ULONG length = email_domains.size();
  HRESULT hr = GetGlobalFlag(kEmailDomainsKey, &email_domains[0], &length);
  if (FAILED(hr)) {
    if (hr == HRESULT_FROM_WIN32(ERROR_MORE_DATA)) {
      email_domains.resize(length + 1);
      length = email_domains.size();
      hr = GetGlobalFlag(kEmailDomainsKey, &email_domains[0], &length);
      if (FAILED(hr))
        email_domains[0] = 0;
    }
  }
  return base::string16(&email_domains[0]);
}
