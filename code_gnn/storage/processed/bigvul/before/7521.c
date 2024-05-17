  tt_size_ready_bytecode( TT_Size  size,
                          FT_Bool  pedantic )
  {
    FT_Error  error = FT_Err_Ok;


    if ( size->bytecode_ready < 0 )
      error = tt_size_init_bytecode( (FT_Size)size, pedantic );
    else
      error = size->bytecode_ready;

    if ( error )
      goto Exit;

     
    if ( size->cvt_ready < 0 )
    {
      FT_UInt  i;
      TT_Face  face = (TT_Face)size->root.face;


       
       
      for ( i = 0; i < size->cvt_size; i++ )
        size->cvt[i] = FT_MulFix( face->cvt[i], size->ttmetrics.scale );

       
      for ( i = 0; i < (FT_UInt)size->twilight.n_points; i++ )
      {
        size->twilight.org[i].x = 0;
        size->twilight.org[i].y = 0;
        size->twilight.cur[i].x = 0;
        size->twilight.cur[i].y = 0;
      }

       
      for ( i = 0; i < (FT_UInt)size->storage_size; i++ )
        size->storage[i] = 0;

      size->GS = tt_default_graphics_state;

      error = tt_size_run_prep( size, pedantic );
    }
    else
      error = size->cvt_ready;

  Exit:
    return error;
  }
