std::vector<uint8_t> GenerateProofInputV2(
    uint64_t timestamp,
    base::span<const uint8_t, kChallengeLen> client_challenge) {
  NtlmBufferWriter writer(kProofInputLenV2);
  bool result = writer.WriteUInt16(kProofInputVersionV2) &&
                writer.WriteZeros(6) && writer.WriteUInt64(timestamp) &&
                writer.WriteBytes(client_challenge) && writer.WriteZeros(4) &&
                writer.IsEndOfBuffer();

  DCHECK(result);
  return writer.Pass();
}
