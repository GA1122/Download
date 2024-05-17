String securityState(const GURL& url, const net::CertStatus& cert_status) {
  if (!url.SchemeIsCryptographic())
    return Security::SecurityStateEnum::Neutral;
  if (net::IsCertStatusError(cert_status) &&
      !net::IsCertStatusMinorError(cert_status)) {
    return Security::SecurityStateEnum::Insecure;
  }
  return Security::SecurityStateEnum::Secure;
}
