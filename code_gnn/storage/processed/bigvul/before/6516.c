  _bdf_readstream( FT_Stream         stream,
                   _bdf_line_func_t  callback,
                   void*             client_data,
                   unsigned long    *lno )
  {
    _bdf_line_func_t  cb;
    unsigned long     lineno, buf_size;
    int               refill, hold, to_skip;
    ptrdiff_t         bytes, start, end, cursor, avail;
    char*             buf = 0;
    FT_Memory         memory = stream->memory;
    FT_Error          error = BDF_Err_Ok;


    if ( callback == 0 )
    {
      error = BDF_Err_Invalid_Argument;
      goto Exit;
    }

     
    buf_size = 1024;

    if ( FT_NEW_ARRAY( buf, buf_size ) )
      goto Exit;

    cb      = callback;
    lineno  = 1;
    buf[0]  = 0;
    start   = 0;
    end     = 0;
    avail   = 0;
    cursor  = 0;
    refill  = 1;
    to_skip = NO_SKIP;
    bytes   = 0;         

    for (;;)
    {
      if ( refill )
      {
        bytes  = (ptrdiff_t)FT_Stream_TryRead(
                   stream, (FT_Byte*)buf + cursor,
                   (FT_ULong)( buf_size - cursor ) );
        avail  = cursor + bytes;
        cursor = 0;
        refill = 0;
      }

      end = start;

       
      if ( start < avail && buf[start] == to_skip )
      {
        start  += 1;
        to_skip = NO_SKIP;
        continue;
      }

       
      while ( end < avail && buf[end] != '\n' && buf[end] != '\r' )
        end++;

       
       
      if ( end >= avail )
      {
        if ( bytes == 0 )   
          break;            

        if ( start == 0 )
        {
           
           
          FT_ULong  new_size;


          if ( buf_size >= 65536UL )   
          {
            FT_ERROR(( "_bdf_readstream: " ERRMSG6, lineno ));
            error = BDF_Err_Invalid_Argument;
            goto Exit;
          }

          new_size = buf_size * 2;
          if ( FT_RENEW_ARRAY( buf, buf_size, new_size ) )
            goto Exit;

          cursor   = buf_size;
          buf_size = new_size;
        }
        else
        {
          bytes = avail - start;

          FT_MEM_COPY( buf, buf + start, bytes );

          cursor = bytes;
          avail -= bytes;
          start  = 0;
        }
        refill = 1;
        continue;
      }

       
      hold     = buf[end];
      buf[end] = 0;

       
      if ( buf[start] != '#' && buf[start] != 0x1a && end > start )
      {
        error = (*cb)( buf + start, end - start, lineno,
                       (void*)&cb, client_data );
         
        if ( error == -1 )
          error = (*cb)( buf + start, end - start, lineno,
                         (void*)&cb, client_data );
        if ( error )
          break;
      }

      lineno  += 1;
      buf[end] = (char)hold;
      start    = end + 1;

      if ( hold == '\n' )
        to_skip = '\r';
      else if ( hold == '\r' )
        to_skip = '\n';
      else
        to_skip = NO_SKIP;
    }

    *lno = lineno;

  Exit:
    FT_FREE( buf );
    return error;
  }