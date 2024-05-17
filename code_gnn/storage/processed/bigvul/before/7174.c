  tt_face_load_loca( TT_Face    face,
                     FT_Stream  stream )
  {
    FT_Error  error;
    FT_ULong  table_len;
    FT_Int    shift;


     
    error = face->goto_table( face, TTAG_glyf, stream, &face->glyf_len );

     
     
    if ( FT_ERR_EQ( error, Table_Missing ) )
      face->glyf_len = 0;
    else if ( error )
      goto Exit;

    FT_TRACE2(( "Locations " ));
    error = face->goto_table( face, TTAG_loca, stream, &table_len );
    if ( error )
    {
      error = FT_THROW( Locations_Missing );
      goto Exit;
    }

    if ( face->header.Index_To_Loc_Format != 0 )
    {
      shift = 2;

      if ( table_len >= 0x40000L )
      {
        FT_TRACE2(( "table too large\n" ));
        error = FT_THROW( Invalid_Table );
        goto Exit;
      }
      face->num_locations = table_len >> shift;
    }
    else
    {
      shift = 1;

      if ( table_len >= 0x20000L )
      {
        FT_TRACE2(( "table too large\n" ));
        error = FT_THROW( Invalid_Table );
        goto Exit;
      }
      face->num_locations = table_len >> shift;
    }

    if ( face->num_locations != (FT_ULong)face->root.num_glyphs + 1 )
    {
      FT_TRACE2(( "glyph count mismatch!  loca: %d, maxp: %d\n",
                  face->num_locations - 1, face->root.num_glyphs ));

       
      if ( face->num_locations <= (FT_ULong)face->root.num_glyphs )
      {
        FT_Long   new_loca_len =
                    ( (FT_Long)( face->root.num_glyphs ) + 1 ) << shift;

        TT_Table  entry = face->dir_tables;
        TT_Table  limit = entry + face->num_tables;

        FT_Long   pos  = FT_Stream_Pos( stream );
        FT_Long   dist = 0x7FFFFFFFL;


         
        for ( ; entry < limit; entry++ )
        {
          FT_Long  diff = entry->Offset - pos;


          if ( diff > 0 && diff < dist )
            dist = diff;
        }

        if ( entry == limit )
        {
           
          dist = stream->size - pos;
        }

        if ( new_loca_len <= dist )
        {
          face->num_locations = face->root.num_glyphs + 1;
          table_len           = new_loca_len;

          FT_TRACE2(( "adjusting num_locations to %d\n",
                      face->num_locations ));
        }
      }
    }

     
    if ( FT_FRAME_EXTRACT( table_len, face->glyph_locations ) )
      goto Exit;

    FT_TRACE2(( "loaded\n" ));

  Exit:
    return error;
  }
