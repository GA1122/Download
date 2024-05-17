void GenerateNtlmResponseV1(
    const base::string16& password,
    base::span<const uint8_t, kChallengeLen> server_challenge,
    base::span<uint8_t, kResponseLenV1> ntlm_response) {
  uint8_t ntlm_hash[kNtlmHashLen];
  GenerateNtlmHashV1(password, ntlm_hash);
  GenerateResponseDesl(ntlm_hash, server_challenge, ntlm_response);
}
