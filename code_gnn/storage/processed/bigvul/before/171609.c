int32_t utf32_from_utf8_at(const char *src, size_t src_len, size_t index, size_t *next_index)
{
 if (index >= src_len) {
 return -1;
 }
 size_t dummy_index;
 if (next_index == NULL) {
        next_index = &dummy_index;
 }
 size_t num_read;
 int32_t ret = utf32_at_internal(src + index, &num_read);
 if (ret >= 0) {
 *next_index = index + num_read;
 }

 return ret;
}
