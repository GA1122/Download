void utf8_to_utf32(const char* src, size_t src_len, char32_t* dst)
{
 if (src == NULL || src_len == 0 || dst == NULL) {
 return;
 }

 const char* cur = src;
 const char* const end = src + src_len;
 char32_t* cur_utf32 = dst;
 while (cur < end) {
 size_t num_read;
 *cur_utf32++ = static_cast<char32_t>(utf32_at_internal(cur, &num_read));
        cur += num_read;
 }
 *cur_utf32 = 0;
}
