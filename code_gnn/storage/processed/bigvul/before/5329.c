free_all_not_allocator(clump_t *cp, void *arg)
{
    struct free_data *fd = (struct free_data *)arg;

    if (cp->cbase + sizeof(obj_header_t) != (byte *)fd->imem)
        alloc_free_clump(cp, fd->imem);
    else
        fd->allocator = cp;

    return SPLAY_APP_CONTINUE;
}
