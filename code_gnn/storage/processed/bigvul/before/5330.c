gs_id get_mem_hdr_id (void *ptr)
{
    return (*((hdr_id_t *)((byte *)ptr) - HDR_ID_OFFSET));
}
