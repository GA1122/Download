  cff_index_access_element( CFF_Index  idx,
                            FT_UInt    element,
                            FT_Byte**  pbytes,
                            FT_ULong*  pbyte_len )
  {
    FT_Error  error = CFF_Err_Ok;


    if ( idx && idx->count > element )
    {
       
      FT_Stream  stream = idx->stream;
      FT_ULong   off1, off2 = 0;


       
      if ( !idx->offsets )
      {
        FT_ULong  pos = element * idx->off_size;


        if ( FT_STREAM_SEEK( idx->start + 3 + pos ) )
          goto Exit;

        off1 = cff_index_read_offset( idx, &error );
        if ( error )
          goto Exit;

        if ( off1 != 0 )
        {
          do
          {
            element++;
            off2 = cff_index_read_offset( idx, &error );
          }
          while ( off2 == 0 && element < idx->count );
        }
      }
      else    
      {
        off1 = idx->offsets[element];
        if ( off1 )
        {
          do
          {
            element++;
            off2 = idx->offsets[element];

          } while ( off2 == 0 && element < idx->count );
        }
      }

       
      if ( off1 && off2 > off1 )
      {
        *pbyte_len = off2 - off1;

        if ( idx->bytes )
        {
           
          *pbytes = idx->bytes + off1 - 1;
        }
        else
        {
           
          if ( FT_STREAM_SEEK( idx->data_offset + off1 - 1 ) ||
               FT_FRAME_EXTRACT( off2 - off1, *pbytes )      )
            goto Exit;
        }
      }
      else
      {
         
        *pbytes    = 0;
        *pbyte_len = 0;
      }
    }
    else
      error = CFF_Err_Invalid_Argument;

  Exit:
    return error;
  }
