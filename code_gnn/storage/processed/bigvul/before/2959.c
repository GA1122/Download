pdf14_mask_element_new(gs_memory_t *memory)
{
    pdf14_mask_t *result;

    result = gs_alloc_struct(memory, pdf14_mask_t, &st_pdf14_mask,
                             "pdf14_mask_element_new");
     
    result->rc_mask = NULL;
    result->previous = NULL;
    result->memory = memory;
    return(result);
}
