static int bdrv_open_flags(BlockDriverState *bs, int flags)
{
    int open_flags = flags | BDRV_O_CACHE_WB;

     
    open_flags &= ~(BDRV_O_SNAPSHOT | BDRV_O_NO_BACKING);

     
    if (bs->is_temporary) {
        open_flags |= BDRV_O_RDWR;
    }

    return open_flags;
}
