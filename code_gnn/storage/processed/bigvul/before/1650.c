bitmap_paint(gs_image_enum * pen, gs_data_image_t * pim,
             const gs_depth_bitmap * pbitmap, gs_gstate * pgs)
{
    uint raster = pbitmap->raster;
    uint nbytes = (pim->Width * pbitmap->pix_depth + 7) >> 3;
    uint used;
    const byte *dp = pbitmap->data;
    int n;
    int code = 0, code1;

    if (nbytes == raster)
        code = gs_image_next(pen, dp, nbytes * pim->Height, &used);
    else
        for (n = pim->Height; n > 0 && code >= 0; dp += raster, --n)
            code = gs_image_next(pen, dp, nbytes, &used);
    code1 = gs_image_cleanup_and_free_enum(pen, pgs);
    if (code >= 0 && code1 < 0)
        code = code1;
    return code;
}
