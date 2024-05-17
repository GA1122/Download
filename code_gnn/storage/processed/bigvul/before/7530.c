  woff_open_font( FT_Stream  stream,
                  TT_Face    face )
  {
    FT_Memory       memory = stream->memory;
    FT_Error        error  = FT_Err_Ok;

    WOFF_HeaderRec  woff;
    WOFF_Table      tables  = NULL;
    WOFF_Table*     indices = NULL;

    FT_ULong        woff_offset;

    FT_Byte*        sfnt        = NULL;
    FT_Stream       sfnt_stream = NULL;

    FT_Byte*        sfnt_header;
    FT_ULong        sfnt_offset;

    FT_Int          nn;
    FT_ULong        old_tag = 0;

    static const FT_Frame_Field  woff_header_fields[] =
    {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  WOFF_HeaderRec

      FT_FRAME_START( 44 ),
        FT_FRAME_ULONG ( signature ),
        FT_FRAME_ULONG ( flavor ),
        FT_FRAME_ULONG ( length ),
        FT_FRAME_USHORT( num_tables ),
        FT_FRAME_USHORT( reserved ),
        FT_FRAME_ULONG ( totalSfntSize ),
        FT_FRAME_USHORT( majorVersion ),
        FT_FRAME_USHORT( minorVersion ),
        FT_FRAME_ULONG ( metaOffset ),
        FT_FRAME_ULONG ( metaLength ),
        FT_FRAME_ULONG ( metaOrigLength ),
        FT_FRAME_ULONG ( privOffset ),
        FT_FRAME_ULONG ( privLength ),
      FT_FRAME_END
    };


    FT_ASSERT( stream == face->root.stream );
    FT_ASSERT( FT_STREAM_POS() == 0 );

    if ( FT_STREAM_READ_FIELDS( woff_header_fields, &woff ) )
      return error;

     
    if ( woff.flavor == TTAG_wOFF || woff.flavor == TTAG_ttcf )
      return FT_THROW( Invalid_Table );

     
    if ( woff.length != stream->size                              ||
         woff.num_tables == 0                                     ||
         44 + woff.num_tables * 20UL >= woff.length               ||
         12 + woff.num_tables * 16UL >= woff.totalSfntSize        ||
         ( woff.totalSfntSize & 3 ) != 0                          ||
         ( woff.metaOffset == 0 && ( woff.metaLength != 0     ||
                                     woff.metaOrigLength != 0 ) ) ||
         ( woff.metaLength != 0 && woff.metaOrigLength == 0 )     ||
         ( woff.privOffset == 0 && woff.privLength != 0 )         )
    {
      FT_ERROR(( "woff_font_open: invalid WOFF header\n" ));
      return FT_THROW( Invalid_Table );
    }

     
    if ( FT_ALLOC( sfnt, 12 + woff.num_tables * 16UL ) ||
         FT_NEW( sfnt_stream )                         )
      goto Exit;

    sfnt_header = sfnt;

     
    {
      FT_UInt  searchRange, entrySelector, rangeShift, x;


      x             = woff.num_tables;
      entrySelector = 0;
      while ( x )
      {
        x            >>= 1;
        entrySelector += 1;
      }
      entrySelector--;

      searchRange = ( 1 << entrySelector ) * 16;
      rangeShift  = woff.num_tables * 16 - searchRange;

      WRITE_ULONG ( sfnt_header, woff.flavor );
      WRITE_USHORT( sfnt_header, woff.num_tables );
      WRITE_USHORT( sfnt_header, searchRange );
      WRITE_USHORT( sfnt_header, entrySelector );
      WRITE_USHORT( sfnt_header, rangeShift );
    }

     
     
     

    if ( FT_NEW_ARRAY( tables, woff.num_tables )  ||
         FT_NEW_ARRAY( indices, woff.num_tables ) )
      goto Exit;

    FT_TRACE2(( "\n"
                "  tag    offset    compLen  origLen  checksum\n"
                "  -------------------------------------------\n" ));

    if ( FT_FRAME_ENTER( 20L * woff.num_tables ) )
      goto Exit;

    for ( nn = 0; nn < woff.num_tables; nn++ )
    {
      WOFF_Table  table = tables + nn;

      table->Tag        = FT_GET_TAG4();
      table->Offset     = FT_GET_ULONG();
      table->CompLength = FT_GET_ULONG();
      table->OrigLength = FT_GET_ULONG();
      table->CheckSum   = FT_GET_ULONG();

      FT_TRACE2(( "  %c%c%c%c  %08lx  %08lx  %08lx  %08lx\n",
                  (FT_Char)( table->Tag >> 24 ),
                  (FT_Char)( table->Tag >> 16 ),
                  (FT_Char)( table->Tag >> 8  ),
                  (FT_Char)( table->Tag       ),
                  table->Offset,
                  table->CompLength,
                  table->OrigLength,
                  table->CheckSum ));

      if ( table->Tag <= old_tag )
      {
        FT_FRAME_EXIT();

        FT_ERROR(( "woff_font_open: table tags are not sorted\n" ));
        error = FT_THROW( Invalid_Table );
        goto Exit;
      }

      old_tag     = table->Tag;
      indices[nn] = table;
    }

    FT_FRAME_EXIT();

     

    ft_qsort( indices,
              woff.num_tables,
              sizeof ( WOFF_Table ),
              compare_offsets );

     

    woff_offset = 44 + woff.num_tables * 20L;
    sfnt_offset = 12 + woff.num_tables * 16L;

    for ( nn = 0; nn < woff.num_tables; nn++ )
    {
      WOFF_Table  table = indices[nn];


      if ( table->Offset != woff_offset                         ||
           table->CompLength > woff.length                      ||
           table->Offset > woff.length - table->CompLength      ||
           table->OrigLength > woff.totalSfntSize               ||
           sfnt_offset > woff.totalSfntSize - table->OrigLength ||
           table->CompLength > table->OrigLength                )
      {
        FT_ERROR(( "woff_font_open: invalid table offsets\n" ));
        error = FT_THROW( Invalid_Table );
        goto Exit;
      }

      table->OrigOffset = sfnt_offset;

       
      woff_offset += ( table->CompLength + 3 ) & ~3U;
      sfnt_offset += ( table->OrigLength + 3 ) & ~3U;
    }

     

    if ( woff.metaOffset )
    {
      if ( woff.metaOffset != woff_offset                  ||
           woff.metaOffset + woff.metaLength > woff.length )
      {
        FT_ERROR(( "woff_font_open:"
                   " invalid `metadata' offset or length\n" ));
        error = FT_THROW( Invalid_Table );
        goto Exit;
      }

       
      woff_offset += woff.metaLength;
    }

    if ( woff.privOffset )
    {
       
      woff_offset = ( woff_offset + 3 ) & ~3U;

      if ( woff.privOffset != woff_offset                  ||
           woff.privOffset + woff.privLength > woff.length )
      {
        FT_ERROR(( "woff_font_open: invalid `private' offset or length\n" ));
        error = FT_THROW( Invalid_Table );
        goto Exit;
      }

       
      woff_offset += woff.privLength;
    }

    if ( sfnt_offset != woff.totalSfntSize ||
         woff_offset != woff.length        )
    {
      FT_ERROR(( "woff_font_open: invalid `sfnt' table structure\n" ));
      error = FT_THROW( Invalid_Table );
      goto Exit;
    }

     
    if ( FT_REALLOC( sfnt,
                     12 + woff.num_tables * 16UL,
                     woff.totalSfntSize ) )
      goto Exit;

    sfnt_header = sfnt + 12;

     

    for ( nn = 0; nn < woff.num_tables; nn++ )
    {
      WOFF_Table  table = tables + nn;


       
      WRITE_ULONG( sfnt_header, table->Tag );
      WRITE_ULONG( sfnt_header, table->CheckSum );
      WRITE_ULONG( sfnt_header, table->OrigOffset );
      WRITE_ULONG( sfnt_header, table->OrigLength );

       
      if ( FT_STREAM_SEEK( table->Offset )     ||
           FT_FRAME_ENTER( table->CompLength ) )
        goto Exit;

      if ( table->CompLength == table->OrigLength )
      {
         
        ft_memcpy( sfnt + table->OrigOffset,
                   stream->cursor,
                   table->OrigLength );
      }
      else
      {
#ifdef FT_CONFIG_OPTION_USE_ZLIB

         
        FT_ULong  output_len = table->OrigLength;


        error = FT_Gzip_Uncompress( memory,
                                    sfnt + table->OrigOffset, &output_len,
                                    stream->cursor, table->CompLength );
        if ( error )
          goto Exit;
        if ( output_len != table->OrigLength )
        {
          FT_ERROR(( "woff_font_open: compressed table length mismatch\n" ));
          error = FT_THROW( Invalid_Table );
          goto Exit;
        }

#else  

        error = FT_THROW( Unimplemented_Feature );
        goto Exit;

#endif  
      }

      FT_FRAME_EXIT();

       
       
      sfnt_offset = table->OrigOffset + table->OrigLength;
      while ( sfnt_offset & 3 )
      {
        sfnt[sfnt_offset] = '\0';
        sfnt_offset++;
      }
    }

     
    FT_Stream_OpenMemory( sfnt_stream, sfnt, woff.totalSfntSize );
    sfnt_stream->memory = stream->memory;
    sfnt_stream->close  = sfnt_stream_close;

    FT_Stream_Free(
      face->root.stream,
      ( face->root.face_flags & FT_FACE_FLAG_EXTERNAL_STREAM ) != 0 );

    face->root.stream = sfnt_stream;

    face->root.face_flags &= ~FT_FACE_FLAG_EXTERNAL_STREAM;

  Exit:
    FT_FREE( tables );
    FT_FREE( indices );

    if ( error )
    {
      FT_FREE( sfnt );
      FT_Stream_Close( sfnt_stream );
      FT_FREE( sfnt_stream );
    }

    return error;
  }