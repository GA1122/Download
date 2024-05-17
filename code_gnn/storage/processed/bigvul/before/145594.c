NtlmClient::NtlmClient(NtlmFeatures features)
    : features_(features), negotiate_flags_(kNegotiateMessageFlags) {
  GenerateNegotiateMessage();
}
