gx_device_finalize(const gs_memory_t *cmem, void *vptr)
{
    gx_device * const dev = (gx_device *)vptr;
    (void)cmem;  

    if (dev->icc_struct != NULL) {
        rc_decrement(dev->icc_struct, "gx_device_finalize(icc_profile)");
    }
    if (dev->finalize)
        dev->finalize(dev);

     
    if (dev->child)
        dev->child->parent = dev->parent;
    if (dev->parent)
        dev->parent->child = dev->child;
    if (dev->PageList) {
        rc_decrement(dev->PageList, "gx_device_finalize(PageList)");
        dev->PageList = 0;
    }

    discard(gs_closedevice(dev));
    if (dev->stype_is_dynamic)
        gs_free_const_object(dev->memory->non_gc_memory, dev->stype,
                             "gx_device_finalize");

#ifdef DEBUG
     
    if (gs_debug_c('^'))
        dev->rc.memory = NULL;
#endif
}
