static void makeBitReader(
 const void *data, size_t size,
        ogg_buffer *buf, ogg_reference *ref, oggpack_buffer *bits) {
    buf->data = (uint8_t *)data;
    buf->size = size;
    buf->refcount = 1;
    buf->ptr.owner = NULL;

    ref->buffer = buf;
    ref->begin = 0;
    ref->length = size;
    ref->next = NULL;

    oggpack_readinit(bits, ref);
}
