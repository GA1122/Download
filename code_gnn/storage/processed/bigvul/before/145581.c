void GenerateResponsesV1WithSessionSecurity(
    const base::string16& password,
    base::span<const uint8_t, kChallengeLen> server_challenge,
    base::span<const uint8_t, kChallengeLen> client_challenge,
    base::span<uint8_t, kResponseLenV1> lm_response,
    base::span<uint8_t, kResponseLenV1> ntlm_response) {
  GenerateLMResponseV1WithSessionSecurity(client_challenge, lm_response);
  GenerateNtlmResponseV1WithSessionSecurity(password, server_challenge,
                                            client_challenge, ntlm_response);
}
