bool WriteResponsePayloads(
    NtlmBufferWriter* authenticate_writer,
    base::span<const uint8_t, kResponseLenV1> lm_response,
    base::span<const uint8_t, kResponseLenV1> ntlm_response) {
  return authenticate_writer->WriteBytes(lm_response) &&
         authenticate_writer->WriteBytes(ntlm_response);
}
