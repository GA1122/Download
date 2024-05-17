ssize_t utf32_to_utf8_length(const char32_t *src, size_t src_len)
{
 if (src == NULL || src_len == 0) {
 return -1;
 }

 size_t ret = 0;
 const char32_t *end = src + src_len;
 while (src < end) {
        ret += utf32_codepoint_utf8_length(*src++);
 }

     return ret;
 }
