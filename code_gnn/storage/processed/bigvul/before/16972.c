WebContext::WebContext(WebContextProxyClient* client,
                       QObject* handle)
    : client_(client),
      construct_props_(new ConstructProperties()),
      weak_factory_(this) {
  DCHECK(client);
  DCHECK(handle);

  setHandle(handle);

  delegate_ = new BrowserContextDelegate(weak_factory_.GetWeakPtr());

  static_assert(
      CookiePolicyAllowAll == static_cast<CookiePolicy>(
        net::StaticCookiePolicy::ALLOW_ALL_COOKIES),
      "CookiePolicy and net::StaticCookiePolicy::Type values don't match: "
      "CookiePolicyAllowAll");
  static_assert(
      CookiePolicyBlockAll == static_cast<CookiePolicy>(
        net::StaticCookiePolicy::BLOCK_ALL_COOKIES),
      "CookiePolicy and net::StaticCookiePolicy::Type values don't match: "
      "CookiePolicyBlockAll");
  static_assert(
      CookiePolicyBlockThirdParty == static_cast<CookiePolicy>(
        net::StaticCookiePolicy::BLOCK_ALL_THIRD_PARTY_COOKIES),
      "CookiePolicy and net::StaticCookiePolicy::Type values don't match: "
      "CookiePolicyBlockThirdParty");

  static_assert(
      SessionCookieModeEphemeral == static_cast<SessionCookieMode>(
        content::CookieStoreConfig::EPHEMERAL_SESSION_COOKIES),
      "SessionCookieMode and net::CookieStoreConfig::SessionCookieMode values "
      "don't match: SessionCookieModeEphemeral");
  static_assert(
      SessionCookieModePersistent == static_cast<SessionCookieMode>(
        content::CookieStoreConfig::PERSISTANT_SESSION_COOKIES),
      "SessionCookieMode and net::CookieStoreConfig::SessionCookieMode values "
      "don't match: SessionCookieModePersistent");
  static_assert(
      SessionCookieModeRestored == static_cast<SessionCookieMode>(
        content::CookieStoreConfig::RESTORED_SESSION_COOKIES),
      "SessionCookieMode and net::CookieStoreConfig::SessionCookieMode values "
      "don't match: SessionCookieModeRestored");
}
