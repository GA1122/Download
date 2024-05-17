  FT_Stream_ExtractFrame( FT_Stream  stream,
                          FT_ULong   count,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_Stream_EnterFrame( stream, count );
    if ( !error )
    {
      *pbytes = (FT_Byte*)stream->cursor;

       
      stream->cursor = 0;
      stream->limit  = 0;
    }

    return error;
  }
