std::vector<uint8_t> WriteUpdatedTargetInfo(const std::vector<AvPair>& av_pairs,
                                            size_t updated_target_info_len) {
  bool result = true;
  NtlmBufferWriter writer(updated_target_info_len);
  for (const AvPair& pair : av_pairs) {
    result = writer.WriteAvPair(pair);
    DCHECK(result);
  }

  result = writer.WriteAvPairTerminator() && writer.IsEndOfBuffer();
  DCHECK(result);
  return writer.Pass();
}
