CString TextCodecUTF8::EncodeCommon(const CharType* characters,
                                    wtf_size_t length) {
  CHECK_LE(length, std::numeric_limits<wtf_size_t>::max() / 3);
  Vector<uint8_t> bytes(length * 3);

  wtf_size_t i = 0;
  wtf_size_t bytes_written = 0;
  while (i < length) {
    UChar32 character;
    U16_NEXT(characters, i, length, character);
    if (0xD800 <= character && character <= 0xDFFF)
      character = kReplacementCharacter;
    U8_APPEND_UNSAFE(bytes.data(), bytes_written, character);
  }

  return CString(reinterpret_cast<char*>(bytes.data()), bytes_written);
}
