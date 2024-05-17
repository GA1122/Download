  CFF_Load_FD_Select( CFF_FDSelect  fdselect,
                      FT_UInt       num_glyphs,
                      FT_Stream     stream,
                      FT_ULong      offset )
  {
    FT_Error  error;
    FT_Byte   format;
    FT_UInt   num_ranges;


     
    if ( FT_STREAM_SEEK( offset ) || FT_READ_BYTE( format ) )
      goto Exit;

    fdselect->format      = format;
    fdselect->cache_count = 0;    

    switch ( format )
    {
    case 0:      
      fdselect->data_size = num_glyphs;
      goto Load_Data;

    case 3:      
      if ( FT_READ_USHORT( num_ranges ) )
        goto Exit;

      fdselect->data_size = num_ranges * 3 + 2;

    Load_Data:
      if ( FT_FRAME_EXTRACT( fdselect->data_size, fdselect->data ) )
        goto Exit;
      break;

    default:     
      error = CFF_Err_Invalid_File_Format;
    }

  Exit:
    return error;
  }