const std::vector<std::string>& GetGoogleRegistrableDomains() {
  static base::NoDestructor<std::vector<std::string>>
      kGoogleRegisterableDomains([]() {
        std::vector<std::string> domains;

        std::vector<std::string> tlds{GOOGLE_TLD_LIST};
        for (const std::string& tld : tlds) {
          std::string domain = "google." + tld;

          if (GetDomainAndRegistry(domain,
                                   net::registry_controlled_domains::
                                       INCLUDE_PRIVATE_REGISTRIES) != domain) {
            continue;
          }

          domains.push_back(domain);
        }

        return domains;
      }());

   return *kGoogleRegisterableDomains;
 }
