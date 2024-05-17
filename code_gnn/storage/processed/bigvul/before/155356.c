ChromeContentBrowserClient::GetOriginsRequiringDedicatedProcess() {
  std::vector<url::Origin> isolated_origin_list;

#if !defined(OS_ANDROID)
  isolated_origin_list.emplace_back(
      url::Origin::Create(GaiaUrls::GetInstance()->gaia_url()));
#endif

  return isolated_origin_list;
}
