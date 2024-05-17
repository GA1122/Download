jbig2_dump_huffman_state(Jbig2HuffmanState *hs)
{
    fprintf(stderr, "huffman state %08x %08x offset %d.%d\n", hs->this_word, hs->next_word, hs->offset, hs->offset_bits);
}
