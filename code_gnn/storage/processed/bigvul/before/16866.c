BlockDriverState *bdrv_find_overlay(BlockDriverState *active,
                                    BlockDriverState *bs)
{
    BlockDriverState *overlay = NULL;
    BlockDriverState *intermediate;

    assert(active != NULL);
    assert(bs != NULL);

     
    if (active == bs) {
        return NULL;
    }

    intermediate = active;
    while (intermediate->backing_hd) {
        if (intermediate->backing_hd == bs) {
            overlay = intermediate;
            break;
        }
        intermediate = intermediate->backing_hd;
    }

    return overlay;
}
