static void t1_mark_glyphs(void)
{
    int i;
    char *charset = extra_charset();
    char *g, *s, *r;
    cs_entry *ptr;
    if (t1_synthetic || embed_all_glyphs(tex_font)) {  
        if (cs_tab != NULL)
            for (ptr = cs_tab; ptr < cs_ptr; ptr++)
                if (ptr->valid)
                    ptr->used = true;
        if (subr_tab != NULL) {
            for (ptr = subr_tab; ptr - subr_tab < subr_size; ptr++)
                if (ptr->valid)
                    ptr->used = true;
            subr_max = subr_size - 1;
        }
        return;
    }
    mark_cs(notdef);
    for (i = 0; i < 256; i++)
        if (is_used_char(i)) {
            if (t1_glyph_names[i] == notdef)
                pdftex_warn("character %i is mapped to %s", i, notdef);
            else
                mark_cs(t1_glyph_names[i]);
        }
    if (charset == NULL)
        goto set_subr_max;
    g = s = charset + 1;  
    r = strend(g);
    while (g < r) {
        while (*s != '/' && s < r)
            s++;
        *s = 0;  
        mark_cs(g);
        g = s + 1;
    }
  set_subr_max:
    if (subr_tab != NULL)
        for (subr_max = -1, ptr = subr_tab; ptr - subr_tab < subr_size; ptr++)
            if (ptr->used && ptr - subr_tab > subr_max)
                subr_max = ptr - subr_tab;
}
