size_t NtlmClient::GetAuthenticateHeaderLength() const {
  if (IsNtlmV2()) {
    return kAuthenticateHeaderLenV2;
  }

  return kAuthenticateHeaderLenV1;
}
