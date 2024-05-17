fix_collisions(ASS_Renderer *render_priv, EventImages *imgs, int cnt)
{
    Segment *used = ass_realloc_array(NULL, cnt, sizeof(*used));
    int cnt_used = 0;
    int i, j;

    if (!used)
        return;

    for (i = 0; i < cnt; ++i) {
        ASS_RenderPriv *priv;
        if (!imgs[i].detect_collisions)
            continue;
        priv = get_render_priv(render_priv, imgs[i].event);
        if (priv && priv->height > 0) {  
            Segment s;
            s.a = priv->top;
            s.b = priv->top + priv->height;
            s.ha = priv->left;
            s.hb = priv->left + priv->width;
            if (priv->height != imgs[i].height) {        
                ass_msg(render_priv->library, MSGL_WARN,
                        "Event height has changed");
                priv->top = 0;
                priv->height = 0;
                priv->left = 0;
                priv->width = 0;
            }
            for (j = 0; j < cnt_used; ++j)
                if (overlap(&s, used + j)) {     
                    priv->top = 0;
                    priv->height = 0;
                    priv->left = 0;
                    priv->width = 0;
                }
            if (priv->height > 0) {      
                used[cnt_used].a = priv->top;
                used[cnt_used].b = priv->top + priv->height;
                used[cnt_used].ha = priv->left;
                used[cnt_used].hb = priv->left + priv->width;
                cnt_used++;
                shift_event(render_priv, imgs + i, priv->top - imgs[i].top);
            }
        }
    }
    qsort(used, cnt_used, sizeof(Segment), cmp_segment);

    for (i = 0; i < cnt; ++i) {
        ASS_RenderPriv *priv;
        if (!imgs[i].detect_collisions)
            continue;
        priv = get_render_priv(render_priv, imgs[i].event);
        if (priv && priv->height == 0) {         
            int shift;
            Segment s;
            s.a = imgs[i].top;
            s.b = imgs[i].top + imgs[i].height;
            s.ha = imgs[i].left;
            s.hb = imgs[i].left + imgs[i].width;
            shift = fit_segment(&s, used, &cnt_used, imgs[i].shift_direction);
            if (shift)
                shift_event(render_priv, imgs + i, shift);
            priv->top = imgs[i].top;
            priv->height = imgs[i].height;
            priv->left = imgs[i].left;
            priv->width = imgs[i].width;
        }

    }

    free(used);
}
