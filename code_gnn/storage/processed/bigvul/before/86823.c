static int find_sys_file(struct mschm_decompressor_p *self,
                         struct mschmd_sec_mscompressed *sec,
                         struct mschmd_file **f_ptr, const char *name)
{
    struct mspack_system *sys = self->system;
    struct mschmd_file result;

     
    if (*f_ptr) return MSPACK_ERR_OK;

     
    if (chmd_fast_find((struct mschm_decompressor *) self, sec->base.chm,
                       name, &result, (int)sizeof(result)) || !result.section)
    {
        return MSPACK_ERR_DATAFORMAT;
    }

    if (!(*f_ptr = (struct mschmd_file *) sys->alloc(sys, sizeof(result)))) {
        return MSPACK_ERR_NOMEMORY;
    }

     
    *(*f_ptr) = result;
    (*f_ptr)->filename = (char *) name;

     
    (*f_ptr)->next = sec->base.chm->sysfiles;
    sec->base.chm->sysfiles = *f_ptr;
    return MSPACK_ERR_OK;
}
