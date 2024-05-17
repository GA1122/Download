get_bitmap_glyph(ASS_Renderer *render_priv, GlyphInfo *info)
{
    if (!info->outline || info->symbol == '\n' || info->symbol == 0 || info->skip)
        return;

    BitmapHashValue *val;
    OutlineBitmapHashKey *key = &info->hash_key.u.outline;
    if (ass_cache_get(render_priv->cache.bitmap_cache, &info->hash_key, &val)) {
        info->image = val;
        return;
    }
    if (!val)
        return;

    ASS_Outline *outline = outline_copy(info->outline);
    ASS_Outline *border  = outline_copy(info->border);

    FT_Vector shift = { key->shift_x, key->shift_y };
    double scale_x = render_priv->font_scale_x;
    double fax_scaled = info->fax / info->scale_y * info->scale_x;
    double fay_scaled = info->fay / info->scale_x * info->scale_y;

    transform_3d(shift, outline, border,
                 info->frx, info->fry, info->frz, fax_scaled,
                 fay_scaled, render_priv->blur_scale, info->asc);

    FT_Matrix m = { double_to_d16(scale_x), 0,
                    0, double_to_d16(1.0) };

    if (outline) {
        if (scale_x != 1.0)
            outline_transform(outline, &m);
        outline_translate(outline, key->advance.x, -key->advance.y);
    }
    if (border) {
        if (scale_x != 1.0)
            outline_transform(border, &m);
        outline_translate(border, key->advance.x, -key->advance.y);
    }

    int error = outline_to_bitmap2(render_priv, outline, border,
                                   &val->bm, &val->bm_o);
    if (error)
        info->symbol = 0;

    ass_cache_commit(val, bitmap_size(val->bm) + bitmap_size(val->bm_o) +
                     sizeof(BitmapHashKey) + sizeof(BitmapHashValue));
    info->image = val;

    outline_free(outline);
    free(outline);
    outline_free(border);
    free(border);
}
