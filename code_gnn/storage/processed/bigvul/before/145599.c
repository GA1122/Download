bool WriteStringPayloads(NtlmBufferWriter* authenticate_writer,
                         bool is_unicode,
                         const base::string16& domain,
                         const base::string16& username,
                         const std::string& hostname) {
  if (is_unicode) {
    return authenticate_writer->WriteUtf16String(domain) &&
           authenticate_writer->WriteUtf16String(username) &&
           authenticate_writer->WriteUtf8AsUtf16String(hostname);
  } else {
    return authenticate_writer->WriteUtf16AsUtf8String(domain) &&
           authenticate_writer->WriteUtf16AsUtf8String(username) &&
           authenticate_writer->WriteUtf8String(hostname);
  }
}
