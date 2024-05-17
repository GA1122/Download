  tt_cmap4_char_map_linear( TT_CMap     cmap,
                            FT_UInt32*  pcharcode,
                            FT_Bool     next )
  {
    FT_UInt    num_segs2, start, end, offset;
    FT_Int     delta;
    FT_UInt    i, num_segs;
    FT_UInt32  charcode = *pcharcode;
    FT_UInt    gindex   = 0;
    FT_Byte*   p;


    p = cmap->data + 6;
    num_segs2 = FT_PAD_FLOOR( TT_PEEK_USHORT( p ), 2 );

    num_segs = num_segs2 >> 1;

    if ( !num_segs )
      return 0;

    if ( next )
      charcode++;

     
    for ( ; charcode <= 0xFFFFU; charcode++ )
    {
      FT_Byte*  q;


      p = cmap->data + 14;                
      q = cmap->data + 16 + num_segs2;    

      for ( i = 0; i < num_segs; i++ )
      {
        end   = TT_NEXT_USHORT( p );
        start = TT_NEXT_USHORT( q );

        if ( charcode >= start && charcode <= end )
        {
          p       = q - 2 + num_segs2;
          delta   = TT_PEEK_SHORT( p );
          p      += num_segs2;
          offset  = TT_PEEK_USHORT( p );

           
           
          if ( i >= num_segs - 1                  &&
               start == 0xFFFFU && end == 0xFFFFU )
          {
            TT_Face   face  = (TT_Face)cmap->cmap.charmap.face;
            FT_Byte*  limit = face->cmap_table + face->cmap_size;


            if ( offset && p + offset + 2 > limit )
            {
              delta  = 1;
              offset = 0;
            }
          }

          if ( offset == 0xFFFFU )
            continue;

          if ( offset )
          {
            p += offset + ( charcode - start ) * 2;
            gindex = TT_PEEK_USHORT( p );
            if ( gindex != 0 )
              gindex = (FT_UInt)( gindex + delta ) & 0xFFFFU;
          }
          else
            gindex = (FT_UInt)( charcode + delta ) & 0xFFFFU;

          break;
        }
      }

      if ( !next || gindex )
        break;
    }

    if ( next && gindex )
      *pcharcode = charcode;

    return gindex;
  }
