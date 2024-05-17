  FT_Stream_ExitFrame( FT_Stream  stream )
  {
     
     
     
     
     
     
     
     
     
    FT_ASSERT( stream );

    if ( stream->read )
    {
      FT_Memory  memory = stream->memory;

#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, stream->base );
      stream->base = NULL;
#else
      FT_FREE( stream->base );
#endif
    }
    stream->cursor = 0;
    stream->limit  = 0;
  }
