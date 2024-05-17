rtc::scoped_refptr<rtc::RTCCertificate> CreateTestCertificate() {
  rtc::KeyParams params;
  rtc::SSLIdentity* ssl_identity =
      rtc::SSLIdentity::Generate("dummy_certificate", params);
  return rtc::RTCCertificate::Create(
      std::unique_ptr<rtc::SSLIdentity>(ssl_identity));
}
