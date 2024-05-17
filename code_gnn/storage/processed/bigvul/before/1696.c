mask_PaintProc(const gs_client_color * pcolor, gs_gstate * pgs)
{
    const pixmap_info *ppmap = gs_getpattern(pcolor)->client_data;
    const gs_depth_bitmap *pbitmap = &(ppmap->bitmap);
    gs_image_enum *pen =
    gs_image_enum_alloc(gs_gstate_memory(pgs), "mask_PaintProc");
    gs_image1_t mask;

    if (pen == 0)
        return_error(gs_error_VMerror);
    gs_image_t_init_mask(&mask, true);
    mask.Width = pbitmap->size.x;
    mask.Height = pbitmap->size.y;
    gs_image_init(pen, &mask, false, false, pgs);
    return bitmap_paint(pen, (gs_data_image_t *) & mask, pbitmap, pgs);
}
