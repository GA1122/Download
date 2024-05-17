GURL VariationsService::GetVariationsServerURL(
    PrefService* policy_pref_service,
    const std::string& restrict_mode_override) {
  std::string server_url_string(
      base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
          switches::kVariationsServerURL));
  if (server_url_string.empty())
    server_url_string = kDefaultServerUrl;
  GURL server_url = GURL(server_url_string);

  const std::string restrict_param = !restrict_mode_override.empty() ?
      restrict_mode_override : GetRestrictParameterPref(policy_pref_service);
  if (!restrict_param.empty()) {
    server_url = net::AppendOrReplaceQueryParameter(server_url,
                                                    "restrict",
                                                    restrict_param);
  }

  server_url = net::AppendOrReplaceQueryParameter(server_url, "osname",
                                                  GetPlatformString());

  DCHECK(server_url.is_valid());
  return server_url;
}
