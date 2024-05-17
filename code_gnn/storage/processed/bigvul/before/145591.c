size_t NtlmClient::GetNtlmResponseLength(size_t updated_target_info_len) const {
  if (IsNtlmV2()) {
    return kNtlmResponseHeaderLenV2 + updated_target_info_len + 4;
  }

  return kResponseLenV1;
}
