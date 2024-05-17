static inline int32_t utf32_at_internal(const char* cur, size_t *num_read)
{
 const char first_char = *cur;
 if ((first_char & 0x80) == 0) {  
 *num_read = 1;
 return *cur;
 }
    cur++;
 char32_t mask, to_ignore_mask;
 size_t num_to_read = 0;
 char32_t utf32 = first_char;
 for (num_to_read = 1, mask = 0x40, to_ignore_mask = 0xFFFFFF80;
 (first_char & mask);
         num_to_read++, to_ignore_mask |= mask, mask >>= 1) {
        utf32 = (utf32 << 6) + (*cur++ & 0x3F);
 }
    to_ignore_mask |= mask;
    utf32 &= ~(to_ignore_mask << (6 * (num_to_read - 1)));

 *num_read = num_to_read;
 return static_cast<int32_t>(utf32);
}
