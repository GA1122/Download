static CertStatus ExpectedCertStatusForFailedOnlineRevocationCheck() {
#if defined(OS_WIN)
  return CERT_STATUS_UNABLE_TO_CHECK_REVOCATION;
#else
  return 0;
#endif
}
