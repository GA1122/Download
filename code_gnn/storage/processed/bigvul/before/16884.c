size_t bdrv_opt_mem_align(BlockDriverState *bs)
{
    if (!bs || !bs->drv) {
         
        return 4096;
    }

    return bs->bl.opt_mem_alignment;
}
