void bdrv_detach_dev(BlockDriverState *bs, void *dev)
 
{
    assert(bs->dev == dev);
    bs->dev = NULL;
    bs->dev_ops = NULL;
    bs->dev_opaque = NULL;
    bs->guest_block_size = 512;
}
