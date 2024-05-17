free_all_allocator(clump_t *cp, void *arg)
{
    struct free_data *fd = (struct free_data *)arg;

    if (cp->cbase + sizeof(obj_header_t) != (byte *)fd->imem)
        return SPLAY_APP_CONTINUE;

    fd->allocator = cp;
    alloc_free_clump(cp, fd->imem);
    return SPLAY_APP_STOP;
}
