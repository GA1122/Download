size_t utf8_to_utf32_length(const char *src, size_t src_len)
{
 if (src == NULL || src_len == 0) {
 return 0;
 }
 size_t ret = 0;
 const char* cur;
 const char* end;
 size_t num_to_skip;
 for (cur = src, end = src + src_len, num_to_skip = 1;
         cur < end;
         cur += num_to_skip, ret++) {
 const char first_char = *cur;
        num_to_skip = 1;
 if ((first_char & 0x80) == 0) {  
 continue;
 }
 int32_t mask;

 for (mask = 0x40; (first_char & mask); num_to_skip++, mask >>= 1) {
 }
 }
 return ret;
}
