  tt_cmap13_next( TT_CMap13  cmap )
  {
    FT_Byte*  p;
    FT_ULong  start, end, glyph_id, char_code;
    FT_ULong  n;
    FT_UInt   gindex;


    if ( cmap->cur_charcode >= 0xFFFFFFFFUL )
      goto Fail;

    char_code = cmap->cur_charcode + 1;

    for ( n = cmap->cur_group; n < cmap->num_groups; n++ )
    {
      p        = cmap->cmap.data + 16 + 12 * n;
      start    = TT_NEXT_ULONG( p );
      end      = TT_NEXT_ULONG( p );
      glyph_id = TT_PEEK_ULONG( p );

      if ( char_code < start )
        char_code = start;

      if ( char_code <= end )
      {
        gindex = (FT_UInt)glyph_id;

        if ( gindex )
        {
          cmap->cur_charcode = char_code;;
          cmap->cur_gindex   = gindex;
          cmap->cur_group    = n;

          return;
        }
      }
    }

  Fail:
    cmap->valid = 0;
  }
