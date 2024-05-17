void NtlmClient::GenerateNegotiateMessage() {
  NtlmBufferWriter writer(kNegotiateMessageLen);
  bool result =
      writer.WriteMessageHeader(MessageType::kNegotiate) &&
      writer.WriteFlags(negotiate_flags_) &&
      writer.WriteSecurityBuffer(SecurityBuffer(kNegotiateMessageLen, 0)) &&
      writer.WriteSecurityBuffer(SecurityBuffer(kNegotiateMessageLen, 0)) &&
      writer.IsEndOfBuffer();

  DCHECK(result);

  negotiate_message_ = writer.Pass();
}
