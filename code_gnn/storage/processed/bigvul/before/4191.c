  tt_cmap4_set_range( TT_CMap4  cmap,
                      FT_UInt   range_index )
  {
    FT_Byte*  table = cmap->cmap.data;
    FT_Byte*  p;
    FT_UInt   num_ranges = cmap->num_ranges;


    while ( range_index < num_ranges )
    {
      FT_UInt  offset;


      p             = table + 14 + range_index * 2;
      cmap->cur_end = FT_PEEK_USHORT( p );

      p              += 2 + num_ranges * 2;
      cmap->cur_start = FT_PEEK_USHORT( p );

      p              += num_ranges * 2;
      cmap->cur_delta = FT_PEEK_SHORT( p );

      p     += num_ranges * 2;
      offset = FT_PEEK_USHORT( p );

       
       
      if ( range_index     >= num_ranges - 1 &&
           cmap->cur_start == 0xFFFFU        &&
           cmap->cur_end   == 0xFFFFU        )
      {
        TT_Face   face  = (TT_Face)cmap->cmap.cmap.charmap.face;
        FT_Byte*  limit = face->cmap_table + face->cmap_size;


        if ( offset && p + offset + 2 > limit )
        {
          cmap->cur_delta = 1;
          offset          = 0;
        }
      }

      if ( offset != 0xFFFFU )
      {
        cmap->cur_values = offset ? p + offset : NULL;
        cmap->cur_range  = range_index;
        return 0;
      }

       
      range_index++;
    }

    return -1;
  }
