static inline void utf8_shift_and_mask(uint32_t* codePoint, const uint8_t byte)
{
 *codePoint <<= 6;
 *codePoint |= 0x3F & byte;
}
