void utf8_to_utf16(const uint8_t* u8str, size_t u8len, char16_t* u16str) {
 char16_t* end = utf8_to_utf16_no_null_terminator(u8str, u8len, u16str);
 *end = 0;
}
