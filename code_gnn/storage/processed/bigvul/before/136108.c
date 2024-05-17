bool CertificateTransparencyStatusMatch(
    const content::SignedCertificateTimestampIDStatusList& scts,
    net::ct::SCTVerifyStatus status) {
  for (content::SignedCertificateTimestampIDStatusList::const_iterator it =
           scts.begin();
       it != scts.end();
       ++it) {
    if (it->status == status)
      return true;
  }

  return false;
}
