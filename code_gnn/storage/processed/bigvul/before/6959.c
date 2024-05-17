  read_lwfn( FT_Memory      memory,
             ResFileRefNum  res,
             FT_Byte**      pfb_data,
             FT_ULong*      size )
  {
    FT_Error       error = FT_Err_Ok;
    ResID          res_id;
    unsigned char  *buffer, *p, *size_p = NULL;
    FT_ULong       total_size = 0;
    FT_ULong       old_total_size = 0;
    FT_ULong       post_size, pfb_chunk_size;
    Handle         post_data;
    char           code, last_code;


    UseResFile( res );

     
     
    res_id    = 501;
    last_code = -1;

    for (;;)
    {
      post_data = Get1Resource( TTAG_POST, res_id++ );
      if ( post_data == NULL )
        break;   

      code = (*post_data)[0];

      if ( code != last_code )
      {
        if ( code == 5 )
          total_size += 2;  
        else
          total_size += 6;  
      }

      total_size += GetHandleSize( post_data ) - 2;
      last_code = code;

       
      if ( total_size < old_total_size )
      {
        error = FT_THROW( Array_Too_Large );
        goto Error;
      }

      old_total_size = total_size;
    }

    if ( FT_ALLOC( buffer, (FT_Long)total_size ) )
      goto Error;

     
     
    p              = buffer;
    res_id         = 501;
    last_code      = -1;
    pfb_chunk_size = 0;

    for (;;)
    {
      post_data = Get1Resource( TTAG_POST, res_id++ );
      if ( post_data == NULL )
        break;   

      post_size = (FT_ULong)GetHandleSize( post_data ) - 2;
      code = (*post_data)[0];

      if ( code != last_code )
      {
        if ( last_code != -1 )
        {
           
          if ( size_p != NULL )
          {
            *size_p++ = (FT_Byte)(   pfb_chunk_size         & 0xFF );
            *size_p++ = (FT_Byte)( ( pfb_chunk_size >> 8  ) & 0xFF );
            *size_p++ = (FT_Byte)( ( pfb_chunk_size >> 16 ) & 0xFF );
            *size_p++ = (FT_Byte)( ( pfb_chunk_size >> 24 ) & 0xFF );
          }
          pfb_chunk_size = 0;
        }

        *p++ = 0x80;
        if ( code == 5 )
          *p++ = 0x03;   
        else if ( code == 2 )
          *p++ = 0x02;   
        else
          *p++ = 0x01;   

        if ( code != 5 )
        {
          size_p = p;    
          p += 4;        
        }
      }

      ft_memcpy( p, *post_data + 2, post_size );
      pfb_chunk_size += post_size;
      p += post_size;
      last_code = code;
    }

    *pfb_data = buffer;
    *size = total_size;

  Error:
    CloseResFile( res );
    return error;
  }