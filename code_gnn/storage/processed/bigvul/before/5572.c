xps_flush_text_buffer(xps_context_t *ctx, xps_font_t *font,
        xps_text_buffer_t *buf, int is_charpath)
{
    gs_text_params_t params;
    gs_text_enum_t *textenum;
    float initial_x, x = buf->x[0];
    float initial_y, y = buf->y[0];
    int code;
    int i;
    gs_gstate_color saved;


    initial_x = x;
    initial_y = y;

    params.operation = TEXT_FROM_GLYPHS | TEXT_REPLACE_WIDTHS;
    if (is_charpath)
        params.operation |= TEXT_DO_FALSE_CHARPATH;
    else
        params.operation |= TEXT_DO_DRAW;
    params.data.glyphs = buf->g;
    params.size = buf->count;
    params.x_widths = buf->x + 1;
    params.y_widths = buf->y + 1;
    params.widths_size = buf->count;

    for (i = 0; i < buf->count; i++)
    {
        buf->x[i] = buf->x[i] - x;
        buf->y[i] = buf->y[i] - y;
        x += buf->x[i];
        y += buf->y[i];
    }
    buf->x[buf->count] = 0;
    buf->y[buf->count] = 0;

    if (ctx->pgs->text_rendering_mode == 2 ) {
        gs_text_enum_t *Tr_textenum;
        gs_text_params_t Tr_params;

         
        saved = ctx->pgs->color[1];
         
        ctx->pgs->color[1] = ctx->pgs->color[0];

        if (PreserveTrMode(ctx) != 1) {
             
            gs_moveto(ctx->pgs, initial_x, initial_y);
            Tr_params.operation = TEXT_FROM_GLYPHS | TEXT_REPLACE_WIDTHS | TEXT_DO_TRUE_CHARPATH;
            Tr_params.data.glyphs = params.data.glyphs;
            Tr_params.size = params.size;
            Tr_params.x_widths = params.x_widths;
            Tr_params.y_widths = params.y_widths;
            Tr_params.widths_size = params.widths_size;

            code = gs_text_begin(ctx->pgs, &Tr_params, ctx->memory, &Tr_textenum);
            if (code != 0)
                return gs_throw1(-1, "cannot gs_text_begin() (%d)", code);

            code = gs_text_process(Tr_textenum);

            if (code != 0)
                return gs_throw1(-1, "cannot gs_text_process() (%d)", code);

            gs_text_release(Tr_textenum, "gslt font render");

            gs_stroke(ctx->pgs);
        } else {
             
            gs_gstate *pgs = ctx->pgs;
            double scale = 1;

             
            if (is_xxyy(&pgs->ctm)) {
                scale = fabs(pgs->ctm.xx);
            } else if (is_xyyx(&pgs->ctm)) {
                scale = fabs(pgs->ctm.xy);
            } else if ((pgs->ctm.xx == pgs->ctm.yy && pgs->ctm.xy == -pgs->ctm.yx) ||
                       (pgs->ctm.xx == -pgs->ctm.yy && pgs->ctm.xy == pgs->ctm.yx)
                ) {
                scale = hypot(pgs->ctm.xx, pgs->ctm.xy);
            }
            gs_setlinewidth(pgs, pgs->line_params.half_width * scale);
        }
    }

    gs_moveto(ctx->pgs, initial_x, initial_y);
    code = gs_text_begin(ctx->pgs, &params, ctx->memory, &textenum);
    if (code != 0)
        return gs_throw1(-1, "cannot gs_text_begin() (%d)", code);

    code = gs_text_process(textenum);

    if (code != 0)
        return gs_throw1(-1, "cannot gs_text_process() (%d)", code);

    gs_text_release(textenum, "gslt font render");

    buf->count = 0;

    if (ctx->pgs->text_rendering_mode == 2 ) {
         
        ctx->pgs->color[1] = saved;
    }
    return 0;
}
