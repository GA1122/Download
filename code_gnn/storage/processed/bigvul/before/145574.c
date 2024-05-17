void GenerateNtlmHashV1(const base::string16& password,
                        base::span<uint8_t, kNtlmHashLen> hash) {
  size_t length = password.length() * 2;
  NtlmBufferWriter writer(length);

  bool result = writer.WriteUtf16String(password) && writer.IsEndOfBuffer();
  DCHECK(result);

  MD4(writer.GetBuffer().data(), writer.GetLength(), hash.data());
}
