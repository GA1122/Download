bool ParseChallengeMessage(
    base::span<const uint8_t> challenge_message,
    NegotiateFlags* challenge_flags,
    base::span<uint8_t, kChallengeLen> server_challenge) {
  NtlmBufferReader challenge_reader(challenge_message);

  return challenge_reader.MatchMessageHeader(MessageType::kChallenge) &&
         challenge_reader.SkipSecurityBufferWithValidation() &&
         challenge_reader.ReadFlags(challenge_flags) &&
         challenge_reader.ReadBytes(server_challenge);
}
