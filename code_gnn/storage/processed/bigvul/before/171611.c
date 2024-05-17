static inline size_t utf8_codepoint_len(uint8_t ch)
{
 return ((0xe5000000 >> ((ch >> 3) & 0x1e)) & 3) + 1;
}
