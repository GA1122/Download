const std::string VPNProviderTypeToString(
    chromeos::ProviderType provider_type) {
  switch (provider_type) {
    case chromeos::PROVIDER_TYPE_L2TP_IPSEC_PSK:
      return std::string("L2TP_IPSEC_PSK");
    case chromeos::PROVIDER_TYPE_L2TP_IPSEC_USER_CERT:
      return std::string("L2TP_IPSEC_USER_CERT");
    case chromeos::PROVIDER_TYPE_OPEN_VPN:
      return std::string("OPEN_VPN");
    default:
      return std::string("UNSUPPORTED_PROVIDER_TYPE");
  }
}
