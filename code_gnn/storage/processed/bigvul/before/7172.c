  tt_face_get_location( TT_Face   face,
                        FT_UInt   gindex,
                        FT_UInt  *asize )
  {
    FT_ULong  pos1, pos2;
    FT_Byte*  p;
    FT_Byte*  p_limit;


    pos1 = pos2 = 0;

    if ( gindex < face->num_locations )
    {
      if ( face->header.Index_To_Loc_Format != 0 )
      {
        p       = face->glyph_locations + gindex * 4;
        p_limit = face->glyph_locations + face->num_locations * 4;

        pos1 = FT_NEXT_ULONG( p );
        pos2 = pos1;

        if ( p + 4 <= p_limit )
          pos2 = FT_NEXT_ULONG( p );
      }
      else
      {
        p       = face->glyph_locations + gindex * 2;
        p_limit = face->glyph_locations + face->num_locations * 2;

        pos1 = FT_NEXT_USHORT( p );
        pos2 = pos1;

        if ( p + 2 <= p_limit )
          pos2 = FT_NEXT_USHORT( p );

        pos1 <<= 1;
        pos2 <<= 1;
      }
    }

     
    if ( pos1 > face->glyf_len )
    {
      FT_TRACE1(( "tt_face_get_location:"
                  " too large offset=0x%08lx found for gid=0x%04lx,"
                  " exceeding the end of glyf table (0x%08lx)\n",
                  pos1, gindex, face->glyf_len ));
      *asize = 0;
      return 0;
    }

    if ( pos2 > face->glyf_len )
    {
      FT_TRACE1(( "tt_face_get_location:"
                  " too large offset=0x%08lx found for gid=0x%04lx,"
                  " truncate at the end of glyf table (0x%08lx)\n",
                  pos2, gindex + 1, face->glyf_len ));
      pos2 = face->glyf_len;
    }

     
     
     
     
     
     
     
     
    if ( pos2 >= pos1 )
      *asize = (FT_UInt)( pos2 - pos1 );
    else
      *asize = (FT_UInt)( face->glyf_len - pos1 );

    return pos1;
  }
