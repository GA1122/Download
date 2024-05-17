static void *ivd_aligned_malloc(void *ctxt, WORD32 alignment, WORD32 size) {
    UNUSED(ctxt);
 return memalign(alignment, size);
}
