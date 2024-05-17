  FT_Stream_GetLong( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_Long   result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    if ( p + 3 < stream->limit )
      result       = FT_NEXT_LONG( p );
    stream->cursor = p;
    return result;
  }