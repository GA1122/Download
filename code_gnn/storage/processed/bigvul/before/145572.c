void GenerateLMResponseV1WithSessionSecurity(
    base::span<const uint8_t, kChallengeLen> client_challenge,
    base::span<uint8_t, kResponseLenV1> lm_response) {
  memcpy(lm_response.data(), client_challenge.data(), kChallengeLen);
  memset(lm_response.data() + kChallengeLen, 0, kResponseLenV1 - kChallengeLen);
}
