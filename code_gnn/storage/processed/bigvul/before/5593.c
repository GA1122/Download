xps_encode_font_char_imp(xps_font_t *font, int code)
{
    byte *table;

     
    if (font->cmapsubtable <= 0)
        return code;

    table = font->data + font->cmapsubtable;

    switch (u16(table))
    {
    case 0:  
        return table[code + 6];

    case 4:  
        {
            int segCount2 = u16(table + 6);
            byte *endCount = table + 14;
            byte *startCount = endCount + segCount2 + 2;
            byte *idDelta = startCount + segCount2;
            byte *idRangeOffset = idDelta + segCount2;
            int i2;

            for (i2 = 0; i2 < segCount2 - 3; i2 += 2)
            {
                int delta, roff;
                int start = u16(startCount + i2);
                int glyph;

                if ( code < start )
                    return 0;
                if ( code > u16(endCount + i2) )
                    continue;
                delta = s16(idDelta + i2);
                roff = s16(idRangeOffset + i2);
                if ( roff == 0 )
                {
                    return ( code + delta ) & 0xffff;  
                    return 0;
                }
                glyph = u16(idRangeOffset + i2 + roff + ((code - start) << 1));
                return (glyph == 0 ? 0 : glyph + delta);
            }

             
            return 0;
        }

    case 6:  
        {
            int firstCode = u16(table + 6);
            int entryCount = u16(table + 8);
            if ( code < firstCode || code >= firstCode + entryCount )
                return 0;
            return u16(table + 10 + ((code - firstCode) << 1));
        }

    case 10:  
        {
            int startCharCode = u32(table + 12);
            int numChars = u32(table + 16);
            if ( code < startCharCode || code >= startCharCode + numChars )
                return 0;
            return u32(table + 20 + (code - startCharCode) * 4);
        }

    case 12:  
        {
            int nGroups = u32(table + 12);
            byte *group = table + 16;
            int i;

            for (i = 0; i < nGroups; i++)
            {
                int startCharCode = u32(group + 0);
                int endCharCode = u32(group + 4);
                int startGlyphID = u32(group + 8);
                if ( code < startCharCode )
                    return 0;
                if ( code <= endCharCode )
                    return startGlyphID + (code - startCharCode);
                group += 12;
            }

            return 0;
        }

    case 2:  
    case 8:  
    default:
        gs_warn1("unknown cmap format: %d\n", u16(table));
        return 0;
    }

    return 0;
}
