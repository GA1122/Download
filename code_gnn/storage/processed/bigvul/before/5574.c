xps_parse_glyph_advance(char *s, float *advance, int bidi_level)
{
    bool advance_overridden = false;
    
    if (*s == ',') {
        s = xps_parse_real_num(s + 1, advance, &advance_overridden);

         

        if (advance_overridden && (bidi_level & 1))
            *advance *= -1;
    }
    return s;
}
