bool ParseChallengeMessageV2(
    base::span<const uint8_t> challenge_message,
    NegotiateFlags* challenge_flags,
    base::span<uint8_t, kChallengeLen> server_challenge,
    std::vector<AvPair>* av_pairs) {
  NtlmBufferReader challenge_reader(challenge_message);

  return challenge_reader.MatchMessageHeader(MessageType::kChallenge) &&
         challenge_reader.SkipSecurityBufferWithValidation() &&
         challenge_reader.ReadFlags(challenge_flags) &&
         challenge_reader.ReadBytes(server_challenge) &&
         challenge_reader.SkipBytes(8) &&
         (((*challenge_flags & NegotiateFlags::kTargetInfo) ==
           NegotiateFlags::kTargetInfo)
              ? challenge_reader.ReadTargetInfoPayload(av_pairs)
              : true);
}
