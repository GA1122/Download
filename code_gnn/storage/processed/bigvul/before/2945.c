pdf14_custom_put_image(gx_device * dev, gs_gstate * pgs, gx_device * target)
{
    pdf14_device * pdev = (pdf14_device *)dev;
    pdf14_buf *buf = pdev->ctx->stack;
    gs_int_rect rect = buf->rect;
    int x0 = rect.p.x, y0 = rect.p.y;
    int planestride = buf->planestride;
    int rowstride = buf->rowstride;
    int num_comp = buf->n_chan - 1;
    const byte bg = pdev->ctx->additive ? 0xff : 0;
    int x1, y1, width, height;
    byte *buf_ptr;

    if_debug0m('v', dev->memory, "[v]pdf14_custom_put_image\n");
    rect_intersect(rect, buf->dirty);
    x1 = min(pdev->width, rect.q.x);
    y1 = min(pdev->height, rect.q.y);
    width = x1 - rect.p.x;
    height = y1 - rect.p.y;
    if (width <= 0 || height <= 0 || buf->data == NULL)
        return 0;
    buf_ptr = buf->data + rect.p.y * buf->rowstride + rect.p.x;

    return gx_put_blended_image_custom(target, buf_ptr,
                      planestride, rowstride,
                      x0, y0, width, height, num_comp, bg);
}
