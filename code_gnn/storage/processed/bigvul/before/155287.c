std::unique_ptr<net::ClientCertIdentity> AutoSelectCertificate(
    Profile* profile,
    const GURL& requesting_url,
    net::ClientCertIdentityList& client_certs) {
  HostContentSettingsMap* host_content_settings_map =
      HostContentSettingsMapFactory::GetForProfile(profile);
  std::unique_ptr<base::Value> setting =
      host_content_settings_map->GetWebsiteSetting(
          requesting_url, requesting_url,
          CONTENT_SETTINGS_TYPE_AUTO_SELECT_CERTIFICATE, std::string(), NULL);

  if (!setting || !setting->is_dict())
    return nullptr;

  const base::Value* filters =
      setting->FindKeyOfType("filters", base::Value::Type::LIST);
  if (!filters) {
    host_content_settings_map->SetWebsiteSettingDefaultScope(
        requesting_url, requesting_url,
        CONTENT_SETTINGS_TYPE_AUTO_SELECT_CERTIFICATE, std::string(), nullptr);
    return nullptr;
  }

  for (const base::Value& filter : filters->GetList()) {
    DCHECK(filter.is_dict());

    auto issuer_pattern = ParseCertificatePrincipalPattern(
        filter.FindKeyOfType("ISSUER", base::Value::Type::DICTIONARY));
    auto subject_pattern = ParseCertificatePrincipalPattern(
        filter.FindKeyOfType("SUBJECT", base::Value::Type::DICTIONARY));
    for (auto& client_cert : client_certs) {
      if (issuer_pattern.Matches(client_cert->certificate()->issuer()) &&
          subject_pattern.Matches(client_cert->certificate()->subject())) {
        return std::move(client_cert);
      }
    }
  }

  return nullptr;
}
