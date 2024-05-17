  FT_Stream_ReadFields( FT_Stream              stream,
                        const FT_Frame_Field*  fields,
                        void*                  structure )
  {
    FT_Error  error;
    FT_Bool   frame_accessed = 0;
    FT_Byte*  cursor;

    if ( !fields || !stream )
      return FT_Err_Invalid_Argument;

    cursor = stream->cursor;

    error = FT_Err_Ok;
    do
    {
      FT_ULong  value;
      FT_Int    sign_shift;
      FT_Byte*  p;


      switch ( fields->value )
      {
      case ft_frame_start:   
        error = FT_Stream_EnterFrame( stream, fields->offset );
        if ( error )
          goto Exit;

        frame_accessed = 1;
        cursor         = stream->cursor;
        fields++;
        continue;   

      case ft_frame_bytes:   
      case ft_frame_skip:    
        {
          FT_UInt  len = fields->size;


          if ( cursor + len > stream->limit )
          {
            error = FT_Err_Invalid_Stream_Operation;
            goto Exit;
          }

          if ( fields->value == ft_frame_bytes )
          {
            p = (FT_Byte*)structure + fields->offset;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          continue;
        }

      case ft_frame_byte:
      case ft_frame_schar:   
        value = FT_NEXT_BYTE(cursor);
        sign_shift = 24;
        break;

      case ft_frame_short_be:
      case ft_frame_ushort_be:   
        value = FT_NEXT_USHORT(cursor);
        sign_shift = 16;
        break;

      case ft_frame_short_le:
      case ft_frame_ushort_le:   
        value = FT_NEXT_USHORT_LE(cursor);
        sign_shift = 16;
        break;

      case ft_frame_long_be:
      case ft_frame_ulong_be:   
        value = FT_NEXT_ULONG(cursor);
        sign_shift = 0;
        break;

      case ft_frame_long_le:
      case ft_frame_ulong_le:   
        value = FT_NEXT_ULONG_LE(cursor);
        sign_shift = 0;
        break;

      case ft_frame_off3_be:
      case ft_frame_uoff3_be:   
        value = FT_NEXT_UOFF3(cursor);
        sign_shift = 8;
        break;

      case ft_frame_off3_le:
      case ft_frame_uoff3_le:   
        value = FT_NEXT_UOFF3_LE(cursor);
        sign_shift = 8;
        break;

      default:
         
        stream->cursor = cursor;
        goto Exit;
      }

       
      if ( fields->value & FT_FRAME_OP_SIGNED )
        value = (FT_ULong)( (FT_Int32)( value << sign_shift ) >> sign_shift );

       

      p = (FT_Byte*)structure + fields->offset;
      switch ( fields->size )
      {
      case (8 / FT_CHAR_BIT):
        *(FT_Byte*)p = (FT_Byte)value;
        break;

      case (16 / FT_CHAR_BIT):
        *(FT_UShort*)p = (FT_UShort)value;
        break;

      case (32 / FT_CHAR_BIT):
        *(FT_UInt32*)p = (FT_UInt32)value;
        break;

      default:   
        *(FT_ULong*)p = (FT_ULong)value;
      }

       
      fields++;
    }
    while ( 1 );

  Exit:
     
    if ( frame_accessed )
      FT_Stream_ExitFrame( stream );

    return error;
  }
