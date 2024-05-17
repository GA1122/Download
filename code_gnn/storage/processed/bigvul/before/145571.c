void GenerateChannelBindingHashV2(
    const std::string& channel_bindings,
    base::span<uint8_t, kNtlmHashLen> channel_bindings_hash) {
  NtlmBufferWriter writer(kEpaUnhashedStructHeaderLen);
  bool result = writer.WriteZeros(16) &&
                writer.WriteUInt32(channel_bindings.length()) &&
                writer.IsEndOfBuffer();
  DCHECK(result);

  MD5_CTX ctx;
  MD5_Init(&ctx);
  MD5_Update(&ctx, writer.GetBuffer().data(), writer.GetBuffer().size());
  MD5_Update(&ctx, channel_bindings.data(), channel_bindings.size());
  MD5_Final(channel_bindings_hash.data(), &ctx);
}
