void GenerateResponsesV1(
    const base::string16& password,
    base::span<const uint8_t, kChallengeLen> server_challenge,
    base::span<uint8_t, kResponseLenV1> lm_response,
    base::span<uint8_t, kResponseLenV1> ntlm_response) {
  GenerateNtlmResponseV1(password, server_challenge, ntlm_response);

  memcpy(lm_response.data(), ntlm_response.data(), kResponseLenV1);
}
