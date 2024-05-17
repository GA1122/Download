jbig2_huffman_new(Jbig2Ctx *ctx, Jbig2WordStream *ws)
{
    Jbig2HuffmanState *result = NULL;

    result = jbig2_new(ctx, Jbig2HuffmanState, 1);

    if (result != NULL) {
        result->offset = 0;
        result->offset_bits = 0;
        result->offset_limit = 0;
        result->ws = ws;
        result->ctx = ctx;
        result->this_word = huff_get_next_word(result, 0);
        result->next_word = huff_get_next_word(result, 4);
    } else {
        jbig2_error(ctx, JBIG2_SEVERITY_FATAL, -1, "failed to allocate new huffman coding state");
    }

    return result;
}
