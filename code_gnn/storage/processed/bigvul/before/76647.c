static char **t1_builtin_enc(void)
{
    int i, a, b, c, counter = 0;
    char *r, *p, **glyph_names;
     
    glyph_names = t1_builtin_glyph_names;
    for (i = 0; i < 256; i++)
        glyph_names[i] = notdef;
    if (t1_suffix("def")) {      
        if (sscanf(t1_line_array + strlen("/Encoding"), "%255s", t1_buf_array) == 1
            && strcmp(t1_buf_array, "StandardEncoding") == 0) {
            t1_encoding = ENC_STANDARD;
            for (i = 0; i < 256; i++) {
                if (standard_glyph_names[i] != notdef)
                    glyph_names[i] = xstrdup(standard_glyph_names[i]);
            }
            return glyph_names;
        }
        pdftex_fail("cannot subset font (unknown predefined encoding `%s')",
                    t1_buf_array);
    }
     
    t1_encoding = ENC_BUILTIN;
    if (t1_prefix("/Encoding [") || t1_prefix("/Encoding[")) {   
        r = strchr(t1_line_array, '[') + 1;
        skip(r, ' ');
        for (;;) {
            while (*r == '/') {
                for (p = t1_buf_array, r++;
                     *r != 32 && *r != 10 && *r != ']' && *r != '/';
                     *p++ = *r++);
                *p = 0;
                skip(r, ' ');
                if (counter > 255)
                    pdftex_fail("encoding vector contains more than 256 names");
                if (strcmp(t1_buf_array, notdef) != 0)
                    glyph_names[counter] = xstrdup(t1_buf_array);
                counter++;
            }
            if (*r != 10 && *r != '%') {
                if (str_prefix(r, "] def") || str_prefix(r, "] readonly def"))
                    break;
                else {
                    remove_eol(r, t1_line_array);
                    pdftex_fail
       ("a name or `] def' or `] readonly def' expected: `%s'", t1_line_array);
                }
            }
            t1_getline();
            r = t1_line_array;
        }
    } else {                     
        p = strchr(t1_line_array, 10);
        for (;;) {
            if (*p == 10) {
                t1_getline();
                p = t1_line_array;
            }
             
            if (sscanf(p, "dup %i%255s put", &i, t1_buf_array) == 2 &&
                *t1_buf_array == '/' && valid_code(i)) {
                if (strcmp(t1_buf_array + 1, notdef) != 0)
                    glyph_names[i] = xstrdup(t1_buf_array + 1);
                p = strstr(p, " put") + strlen(" put");
                skip(p, ' ');
            }
             
            else if (sscanf(p, "dup dup %i exch %i get put", &b, &a) == 2
                     && valid_code(a) && valid_code(b)) {
                copy_glyph_names(glyph_names, a, b);
                p = strstr(p, " get put") + strlen(" get put");
                skip(p, ' ');
            }
             
            else if (sscanf(p, "dup dup %i %i getinterval %i exch putinterval",
                            &a, &c, &b) == 3
                     && valid_code(a) && valid_code(b) && valid_code(c)) {
                for (i = 0; i < c; i++)
                    copy_glyph_names(glyph_names, a + i, b + i);
                p = strstr(p, " putinterval") + strlen(" putinterval");
                skip(p, ' ');
            }
             
            else if ((p == t1_line_array || (p > t1_line_array && p[-1] == ' '))
                     && strcmp(p, "def\n") == 0)
                return glyph_names;
             
            else {
                while (*p != ' ' && *p != 10)
                    p++;
                skip(p, ' ');
            }
        }
    }
    return glyph_names;
}