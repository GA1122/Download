static void qemu_spice_create_one_update(SimpleSpiceDisplay *ssd,
                                         QXLRect *rect)
{
    SimpleSpiceUpdate *update;
    QXLDrawable *drawable;
    QXLImage *image;
    QXLCommand *cmd;
    int bw, bh;
    struct timespec time_space;
    pixman_image_t *dest;

    trace_qemu_spice_create_update(
           rect->left, rect->right,
           rect->top, rect->bottom);

    update   = g_malloc0(sizeof(*update));
    drawable = &update->drawable;
    image    = &update->image;
    cmd      = &update->ext.cmd;

    bw       = rect->right - rect->left;
    bh       = rect->bottom - rect->top;
    update->bitmap = g_malloc(bw * bh * 4);

    drawable->bbox            = *rect;
    drawable->clip.type       = SPICE_CLIP_TYPE_NONE;
    drawable->effect          = QXL_EFFECT_OPAQUE;
    drawable->release_info.id = (uintptr_t)(&update->ext);
    drawable->type            = QXL_DRAW_COPY;
    drawable->surfaces_dest[0] = -1;
    drawable->surfaces_dest[1] = -1;
    drawable->surfaces_dest[2] = -1;
    clock_gettime(CLOCK_MONOTONIC, &time_space);
     
    drawable->mm_time = time_space.tv_sec * 1000
                      + time_space.tv_nsec / 1000 / 1000;

    drawable->u.copy.rop_descriptor  = SPICE_ROPD_OP_PUT;
    drawable->u.copy.src_bitmap      = (uintptr_t)image;
    drawable->u.copy.src_area.right  = bw;
    drawable->u.copy.src_area.bottom = bh;

    QXL_SET_IMAGE_ID(image, QXL_IMAGE_GROUP_DEVICE, ssd->unique++);
    image->descriptor.type   = SPICE_IMAGE_TYPE_BITMAP;
    image->bitmap.flags      = QXL_BITMAP_DIRECT | QXL_BITMAP_TOP_DOWN;
    image->bitmap.stride     = bw * 4;
    image->descriptor.width  = image->bitmap.x = bw;
    image->descriptor.height = image->bitmap.y = bh;
    image->bitmap.data = (uintptr_t)(update->bitmap);
    image->bitmap.palette = 0;
    image->bitmap.format = SPICE_BITMAP_FMT_32BIT;

    dest = pixman_image_create_bits(PIXMAN_x8r8g8b8, bw, bh,
                                    (void *)update->bitmap, bw * 4);
    pixman_image_composite(PIXMAN_OP_SRC, ssd->surface, NULL, ssd->mirror,
                           rect->left, rect->top, 0, 0,
                           rect->left, rect->top, bw, bh);
    pixman_image_composite(PIXMAN_OP_SRC, ssd->mirror, NULL, dest,
                           rect->left, rect->top, 0, 0,
                           0, 0, bw, bh);
    pixman_image_unref(dest);

    cmd->type = QXL_CMD_DRAW;
    cmd->data = (uintptr_t)drawable;

    QTAILQ_INSERT_TAIL(&ssd->updates, update, next);
}
