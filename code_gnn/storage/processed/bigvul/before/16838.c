int bdrv_attach_dev(BlockDriverState *bs, void *dev)
 
{
    if (bs->dev) {
        return -EBUSY;
    }
    bs->dev = dev;
    bdrv_iostatus_reset(bs);
    return 0;
}
