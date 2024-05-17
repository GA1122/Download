bool IsNTPSiteInstance(SiteInstance* site_instance) {
  return site_instance &&
         site_instance->GetSiteURL().SchemeIs(chrome::kChromeSearchScheme) &&
         (site_instance->GetSiteURL().host_piece() ==
              chrome::kChromeSearchRemoteNtpHost ||
          site_instance->GetSiteURL().host_piece() ==
              chrome::kChromeSearchLocalNtpHost);
}
