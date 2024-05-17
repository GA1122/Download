static void t1_subset_ascii_part(void)
{
    int i, j;
    t1_getline();
    while (!t1_prefix("/Encoding")) {
        t1_scan_param();
        if (!(t1_prefix("/UniqueID")
              && !strncmp(t1_line_array + strlen(t1_line_array) -4, "def", 3)))
            t1_putline();
        t1_getline();
    }
    if (is_reencoded(fm_cur))
        t1_glyph_names = external_enc();
    else
        t1_glyph_names = t1_builtin_enc();
    if (is_included(fm_cur) && is_subsetted(fm_cur)) {
        make_subset_tag(fm_cur, t1_glyph_names);
        update_subset_tag();
    }
    if (t1_encoding == ENC_STANDARD)
        t1_puts("/Encoding StandardEncoding def\n");
    else {
        t1_puts
            ("/Encoding 256 array\n0 1 255 {1 index exch /.notdef put} for\n");
        for (i = 0, j = 0; i < 256; i++) {
            if (is_used_char(i) && t1_glyph_names[i] != notdef) {
                j++;
                t1_printf("dup %i /%s put\n", (int)t1_char(i), t1_glyph_names[i]);
            }
        }
        if (j == 0)
             
             
             
            t1_puts("dup 0 /.notdef put\n");
        t1_puts("readonly def\n");
    }
    do {
        t1_getline();
        t1_scan_param();
        if (!t1_prefix("/UniqueID"))     
            t1_putline();
    } while (t1_in_eexec == 0);
}
