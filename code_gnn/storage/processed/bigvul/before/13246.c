  cff_parse_num( CFF_Parser  parser,
                 FT_Byte**   d )
  {
    if ( **d == 30 )
    {
       
      return cff_parse_real( *d, parser->limit, 0, NULL ) >> 16;
    }

    else if ( **d == 255 )
    {
       
       

       
       
      return (FT_Long)( *( (FT_UInt32 *) ( d[0] + 1 ) ) + 0x8000U ) >> 16;
    }

    else
      return cff_parse_integer( *d, parser->limit );
  }
