free_pixmap_pattern(
    gs_memory_t *           pmem,
    void *                  pvpinst,
    client_name_t           cname
)
{
    gs_pattern1_instance_t *pinst = (gs_pattern1_instance_t *)pvpinst;
    pixmap_info *ppmap = pinst->templat.client_data;

    ppmap->free_proc(pmem, pvpinst, cname);
    gs_free_object(pmem, ppmap, cname);
}
