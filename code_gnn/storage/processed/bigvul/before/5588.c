xps_true_callback_glyph_name(gs_font *pfont, gs_glyph glyph, gs_const_string *pstr)
{
     

    int table_length;
    int table_offset;

    ulong format;
    int numGlyphs;
    uint glyph_name_index;
    const byte *postp;  

    if (glyph >= GS_MIN_GLYPH_INDEX) {
        glyph -= GS_MIN_GLYPH_INDEX;
    }

     
    if ( pfont->FontType != ft_TrueType )
    {
        glyph -= 29;
        if (glyph < 258 )
        {
            pstr->data = (byte*) pl_mac_names[glyph];
            pstr->size = strlen((char*)pstr->data);
            return 0;
        }
        else
        {
            return gs_throw1(-1, "glyph index %lu out of range", (ulong)glyph);
        }
    }

    table_offset = xps_find_sfnt_table((xps_font_t*)pfont->client_data, "post", &table_length);

     
    if (table_offset < 0)
        return gs_throw(-1, "no post table");

     
    if ( table_length == 0 )
        return gs_throw(-1, "zero-size post table");

    ((gs_font_type42 *)pfont)->data.string_proc((gs_font_type42 *)pfont,
                                                table_offset, table_length, &postp);
    format = u32(postp);

     
    if ( format != 0x20000 )
    {
         
        char buf[32];
        gs_sprintf(buf, "glyph%d", (int)glyph);
        pstr->data = (byte*)buf;
        pstr->size = strlen((char*)pstr->data);
        return 0;
    }

     
    numGlyphs = (int)u16(postp + 32);
    if ((int)glyph > numGlyphs - 1)
    {
        return gs_throw1(-1, "glyph index %lu out of range", (ulong)glyph);
    }

     
    glyph_name_index = u16(postp + 34 + (glyph * 2));

     
    if ( glyph_name_index > 0x7fff )
        return gs_throw(-1, "post table format error");

     
    if ( glyph_name_index < 258 )
    {
        pstr->data = (byte*) pl_mac_names[glyph_name_index];
        pstr->size = strlen((char*)pstr->data);
        return 0;
    }

     
    else
    {
        byte *mydata;

         
        const byte *pascal_stringp = postp + 34 + (numGlyphs * 2);

         
        glyph_name_index -= 258;

         
        while (glyph_name_index > 0)
        {
            pascal_stringp += ((int)(*pascal_stringp)+1);
            glyph_name_index--;
        }

         
        pstr->size = (int)(*pascal_stringp);

         
        pstr->data = pascal_stringp + 1;

         
        if ( pstr->data + pstr->size > postp + table_length || pstr->data - 1 < postp)
            return gs_throw(-1, "data out of range");

         
        mydata = gs_alloc_bytes(pfont->memory, pstr->size + 1, "glyph to name");
        if ( mydata == 0 )
            return -1;
        memcpy(mydata, pascal_stringp + 1, pstr->size);
        pstr->data = mydata;

        mydata[pstr->size] = 0;

        return 0;
    }
}
