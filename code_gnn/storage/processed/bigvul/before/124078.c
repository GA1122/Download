bool CertMatchesFilter(const net::X509Certificate& cert,
                       const base::DictionaryValue& filter) {

  if (filter.empty())
    return true;

  std::string common_name;
  if (filter.GetString("ISSUER.CN", &common_name) &&
      (cert.issuer().common_name == common_name)) {
    return true;
  }
  return false;
}
