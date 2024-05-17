static bool gx_ttfReader__Eof(ttfReader *self)
{
    gx_ttfReader *r = (gx_ttfReader *)self;

    if (r->extra_glyph_index != -1)
        return r->pos >= r->glyph_data.bits.size;
     
    return false;
}
