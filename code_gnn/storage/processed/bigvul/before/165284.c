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

  if (!setting)
    return nullptr;

  const base::DictionaryValue* setting_dict;
  if (!setting->GetAsDictionary(&setting_dict)) {
    NOTREACHED();
    return nullptr;
  }

  const base::Value* filters =
      setting_dict->FindKeyOfType("filters", base::Value::Type::LIST);
  if (filters) {
    for (const base::Value& filter : filters->GetList()) {
      const base::DictionaryValue* filter_dict;
      if (!filter.GetAsDictionary(&filter_dict)) {
        NOTREACHED();
        continue;
      }
      for (size_t i = 0; i < client_certs.size(); ++i) {
        if (CertMatchesFilter(*client_certs[i]->certificate(), *filter_dict)) {
          return std::move(client_certs[i]);
        }
      }
    }
  } else {
    host_content_settings_map->SetWebsiteSettingDefaultScope(
        requesting_url, requesting_url,
        CONTENT_SETTINGS_TYPE_AUTO_SELECT_CERTIFICATE, std::string(), nullptr);
  }

  return nullptr;
}
