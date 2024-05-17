image_PaintProc(const gs_client_color * pcolor, gs_gstate * pgs)
{
    const pixmap_info *ppmap = gs_getpattern(pcolor)->client_data;
    const gs_depth_bitmap *pbitmap = &(ppmap->bitmap);
    gs_image_enum *pen =
        gs_image_enum_alloc(gs_gstate_memory(pgs), "image_PaintProc");
    gs_color_space *pcspace;
    gx_image_enum_common_t *pie;
     
    int transparent = ppmap->white_index < (1 << (pbitmap->num_comps * pbitmap->pix_depth));

     
    union {
        gs_image1_t i1;
        gs_image4_t i4;
    } image;
    int code;

    if (pen == 0)
        return_error(gs_error_VMerror);

    if (ppmap->pcspace == 0) {
        pcspace = gs_cspace_new_DeviceGray(pgs->memory);
        if (pcspace == NULL)
            return_error(gs_error_VMerror);
    } else
        pcspace = ppmap->pcspace;
    code = gs_gsave(pgs);
    if (code < 0)
        goto fail;
    code = gs_setcolorspace(pgs, pcspace);
    if (code < 0)
        return code;
    if (transparent)
        gs_image4_t_init( (gs_image4_t *) &image, pcspace);
    else
        gs_image_t_init_adjust( (gs_image_t *) &image, pcspace, 0);
    image.i1.Width = pbitmap->size.x;
    image.i1.Height = pbitmap->size.y;
    if (transparent) {
        image.i4.MaskColor_is_range = false;
        image.i4.MaskColor[0] = ppmap->white_index;
    }
    image.i1.Decode[0] = 0.0;
    image.i1.Decode[1] = (float)((1 << pbitmap->pix_depth) - 1);
    image.i1.BitsPerComponent = pbitmap->pix_depth;
     
    if (ppmap->pcspace == 0) {
        image.i1.Decode[0] = 1.0;
        image.i1.Decode[1] = 0.0;
    }

    if ( (code = gs_image_begin_typed( (const gs_image_common_t *)&image,
                                       pgs,
                                       false,
                                       false,
                                       &pie )) >= 0 &&
         (code = gs_image_enum_init( pen,
                                     pie,
                                     (gs_data_image_t *)&image,
                                     pgs )) >= 0 &&
        (code = bitmap_paint(pen, (gs_data_image_t *) & image, pbitmap, pgs)) >= 0) {
        return gs_grestore(pgs);
    }

fail:
    gs_free_object(gs_gstate_memory(pgs), pen, "image_PaintProc");
    return code;
}
