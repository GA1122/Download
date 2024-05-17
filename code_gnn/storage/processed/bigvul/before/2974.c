pdf14_rcmask_new(gs_memory_t *memory)
{
    pdf14_rcmask_t *result;

    result = gs_alloc_struct(memory, pdf14_rcmask_t, &st_pdf14_rcmask,
                             "pdf14_maskbuf_new");
    if ( result == NULL )
        return(NULL);
    rc_init_free(result, memory, 1, rc_pdf14_maskbuf_free);
    result->mask_buf = NULL;
    result->memory = memory;
    return(result);
}
