static inline void byte2hex(const char* data, char** str)
{
 **str = hex_table[(*data >> 4) & 0xf];
 ++*str;
 **str = hex_table[*data & 0xf];
 ++*str;
}
