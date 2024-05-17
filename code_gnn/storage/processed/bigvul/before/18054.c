test_get_word(Jbig2WordStream *self, int offset, uint32_t *word)
{
     
    if (offset + 3 > sizeof(test_stream))
        return -1;
    *word = ((test_stream[offset] << 24) | (test_stream[offset + 1] << 16) | (test_stream[offset + 2] << 8) | (test_stream[offset + 3]));
    return 0;
}
