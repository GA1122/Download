void GenerateNtlmResponseV1WithSessionSecurity(
    const base::string16& password,
    base::span<const uint8_t, kChallengeLen> server_challenge,
    base::span<const uint8_t, kChallengeLen> client_challenge,
    base::span<uint8_t, kResponseLenV1> ntlm_response) {
  uint8_t ntlm_hash[kNtlmHashLen];
  GenerateNtlmHashV1(password, ntlm_hash);

  uint8_t session_hash[kNtlmHashLen];
  GenerateSessionHashV1WithSessionSecurity(server_challenge, client_challenge,
                                           session_hash);

  GenerateResponseDesl(
      ntlm_hash, base::make_span(session_hash).subspan<0, kChallengeLen>(),
      ntlm_response);
}
