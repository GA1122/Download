pdf14_cmykspot_put_image(gx_device * dev, gs_gstate * pgs, gx_device * target)
{
    pdf14_device * pdev = (pdf14_device *)dev;
    pdf14_buf *buf = pdev->ctx->stack;
    gs_int_rect rect = buf->rect;
    int x1, y1, width, height;
    gs_devn_params * pdevn_params = &pdev->devn_params;
    gs_separations * pseparations = &pdevn_params->separations;
    int planestride = buf->planestride;
    int rowstride = buf->rowstride;
    const byte bg = pdev->ctx->additive ? 0xff : 0;
    int num_comp = buf->n_chan - 1;
    byte *buf_ptr;

    if_debug0m('v', dev->memory, "[v]pdf14_cmykspot_put_image\n");
    rect_intersect(rect, buf->dirty);
    x1 = min(pdev->width, rect.q.x);
    y1 = min(pdev->height, rect.q.y);
    width = x1 - rect.p.x;
    height = y1 - rect.p.y;
    if (width <= 0 || height <= 0 || buf->data == NULL)
        return 0;
    buf_ptr = buf->data + rect.p.y * buf->rowstride + rect.p.x;
#if RAW_DUMP
     
    dump_raw_buffer(pdev->ctx->stack->rect.q.y-pdev->ctx->stack->rect.p.y,
                pdev->ctx->stack->rect.q.x-pdev->ctx->stack->rect.p.x,
                                pdev->ctx->stack->n_planes,
                pdev->ctx->stack->planestride, pdev->ctx->stack->rowstride,
                "CMYK_SPOT_PUTIMAGE",pdev->ctx->stack->data);

    global_index++;
    clist_band_count++;
#endif
    return gx_put_blended_image_cmykspot(target, buf_ptr, planestride, rowstride,
                      rect.p.x, rect.p.y, width, height, num_comp, bg,
                      buf->has_tags, rect, pseparations);
}
