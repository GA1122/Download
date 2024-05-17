void GenerateSessionHashV1WithSessionSecurity(
    base::span<const uint8_t, kChallengeLen> server_challenge,
    base::span<const uint8_t, kChallengeLen> client_challenge,
    base::span<uint8_t, kNtlmHashLen> session_hash) {
  MD5_CTX ctx;
  MD5_Init(&ctx);
  MD5_Update(&ctx, server_challenge.data(), kChallengeLen);
  MD5_Update(&ctx, client_challenge.data(), kChallengeLen);
  MD5_Final(session_hash.data(), &ctx);
}
