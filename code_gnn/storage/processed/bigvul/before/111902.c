static bool IsTokenServiceRelevant(const std::string& service) {
  for (int i = 0; i < kRelevantTokenServicesCount; ++i) {
    if (service == kRelevantTokenServices[i])
      return true;
  }
  return false;
}
