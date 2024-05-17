  ps_tostring( FT_Byte**  cursor,
               FT_Byte*   limit,
               FT_Memory  memory )
  {
    FT_Byte*    cur = *cursor;
    FT_UInt     len = 0;
    FT_Int      count;
    FT_String*  result;
    FT_Error    error;


     
     
     
     
     
     
     
     

    while ( cur < limit && ( *cur == ' ' || *cur == '\t' ) )
      cur++;
    if ( cur + 1 >= limit )
      return 0;

    if ( *cur == '(' )
      cur++;   

    *cursor = cur;
    count   = 0;

     
    for ( ; cur < limit; cur++ )
    {
      if ( *cur == '(' )
        count++;

      else if ( *cur == ')' )
      {
        count--;
        if ( count < 0 )
          break;
      }
    }

    len = (FT_UInt)( cur - *cursor );
    if ( cur >= limit || FT_ALLOC( result, len + 1 ) )
      return 0;

     
    FT_MEM_COPY( result, *cursor, len );
    result[len] = '\0';
    *cursor = cur;
    return result;
  }
