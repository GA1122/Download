  ft_var_readpackeddeltas( FT_Stream  stream,
                           FT_Offset  delta_cnt )
  {
    FT_Short  *deltas = NULL;
    FT_UInt    runcnt;
    FT_Offset  i;
    FT_UInt    j;
    FT_Memory  memory = stream->memory;
    FT_Error   error  = TT_Err_Ok;

    FT_UNUSED( error );


    if ( FT_NEW_ARRAY( deltas, delta_cnt ) )
      return NULL;

    i = 0;
    while ( i < delta_cnt )
    {
      runcnt = FT_GET_BYTE();
      if ( runcnt & GX_DT_DELTAS_ARE_ZERO )
      {
         
        for ( j = 0;
              j <= ( runcnt & GX_DT_DELTA_RUN_COUNT_MASK ) && i < delta_cnt;
              ++j )
          deltas[i++] = 0;
      }
      else if ( runcnt & GX_DT_DELTAS_ARE_WORDS )
      {
         
        for ( j = 0;
              j <= ( runcnt & GX_DT_DELTA_RUN_COUNT_MASK ) && i < delta_cnt;
              ++j )
          deltas[i++] = FT_GET_SHORT();
      }
      else
      {
         
        for ( j = 0;
              j <= ( runcnt & GX_DT_DELTA_RUN_COUNT_MASK ) && i < delta_cnt;
              ++j )
          deltas[i++] = FT_GET_CHAR();
      }

      if ( j <= ( runcnt & GX_DT_DELTA_RUN_COUNT_MASK ) )
      {
         
        FT_FREE( deltas );
        return NULL;
      }
    }

    return deltas;
  }
