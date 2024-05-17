WebsiteSettings::SiteIdentityStatus GetSiteIdentityStatusByCTInfo(
    const content::SignedCertificateTimestampIDStatusList& scts,
    bool is_ev) {
  if (CertificateTransparencyStatusMatch(scts, net::ct::SCT_STATUS_INVALID))
    return WebsiteSettings::SITE_IDENTITY_STATUS_ERROR;

  return is_ev ? WebsiteSettings::SITE_IDENTITY_STATUS_EV_CERT
               : WebsiteSettings::SITE_IDENTITY_STATUS_CERT;
}
