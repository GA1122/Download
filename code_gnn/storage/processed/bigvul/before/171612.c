ssize_t utf8_length(const char *src)
{
 const char *cur = src;
 size_t ret = 0;
 while (*cur != '\0') {
 const char first_char = *cur++;
 if ((first_char & 0x80) == 0) {  
            ret += 1;
 continue;
 }
 if ((first_char & 0x40) == 0) {
 return -1;
 }

 int32_t mask, to_ignore_mask;
 size_t num_to_read = 0;
 char32_t utf32 = 0;
 for (num_to_read = 1, mask = 0x40, to_ignore_mask = 0x80;
             num_to_read < 5 && (first_char & mask);
             num_to_read++, to_ignore_mask |= mask, mask >>= 1) {
 if ((*cur & 0xC0) != 0x80) {  
 return -1;
 }
            utf32 = (utf32 << 6) + (*cur++ & 0x3F);
 }
 if (num_to_read == 5) {
 return -1;
 }
        to_ignore_mask |= mask;
        utf32 |= ((~to_ignore_mask) & first_char) << (6 * (num_to_read - 1));
 if (utf32 > kUnicodeMaxCodepoint) {
 return -1;
 }

        ret += num_to_read;
 }
 return ret;
}
