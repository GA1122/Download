  cff_parse_private_dict( CFF_Parser  parser )
  {
    CFF_FontRecDict  dict = (CFF_FontRecDict)parser->object;
    FT_Byte**        data = parser->stack;
    FT_Error         error;


    error = FT_ERR( Stack_Underflow );

    if ( parser->top >= parser->stack + 2 )
    {
      FT_Long  tmp;


      tmp = cff_parse_num( parser, data++ );
      if ( tmp < 0 )
      {
        FT_ERROR(( "cff_parse_private_dict: Invalid dictionary size\n" ));
        error = FT_THROW( Invalid_File_Format );
        goto Fail;
      }
      dict->private_size = (FT_ULong)tmp;

      tmp = cff_parse_num( parser, data );
      if ( tmp < 0 )
      {
        FT_ERROR(( "cff_parse_private_dict: Invalid dictionary offset\n" ));
        error = FT_THROW( Invalid_File_Format );
        goto Fail;
      }
      dict->private_offset = (FT_ULong)tmp;

      FT_TRACE4(( " %lu %lu\n",
                  dict->private_size, dict->private_offset ));

      error = FT_Err_Ok;
    }

  Fail:
    return error;
  }
