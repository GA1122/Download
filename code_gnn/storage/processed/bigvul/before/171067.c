bool ID3::ParseSyncsafeInteger(const uint8_t encoded[4], size_t *x) {
 *x = 0;
 for (int32_t i = 0; i < 4; ++i) {
 if (encoded[i] & 0x80) {
 return false;
 }

 *x = ((*x) << 7) | encoded[i];
 }

 return true;
}
