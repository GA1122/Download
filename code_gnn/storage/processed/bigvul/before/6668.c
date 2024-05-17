  t1_parse_font_matrix( T1_Face    face,
                        T1_Loader  loader )
  {
    T1_Parser   parser = &loader->parser;
    FT_Matrix*  matrix = &face->type1.font_matrix;
    FT_Vector*  offset = &face->type1.font_offset;
    FT_Face     root   = (FT_Face)&face->root;
    FT_Fixed    temp[6];
    FT_Fixed    temp_scale;
    FT_Int      result;


    result = T1_ToFixedArray( parser, 6, temp, 3 );

    if ( result < 6 )
    {
      parser->root.error = FT_THROW( Invalid_File_Format );
      return;
    }

    temp_scale = FT_ABS( temp[3] );

    if ( temp_scale == 0 )
    {
      FT_ERROR(( "t1_parse_font_matrix: invalid font matrix\n" ));
      parser->root.error = FT_THROW( Invalid_File_Format );
      return;
    }

     
     
     

    root->units_per_EM = (FT_UShort)FT_DivFix( 1000, temp_scale );

     
    if ( temp_scale != 0x10000L )
    {
      temp[0] = FT_DivFix( temp[0], temp_scale );
      temp[1] = FT_DivFix( temp[1], temp_scale );
      temp[2] = FT_DivFix( temp[2], temp_scale );
      temp[4] = FT_DivFix( temp[4], temp_scale );
      temp[5] = FT_DivFix( temp[5], temp_scale );
      temp[3] = temp[3] < 0 ? -0x10000L : 0x10000L;
    }

    matrix->xx = temp[0];
    matrix->yx = temp[1];
    matrix->xy = temp[2];
    matrix->yy = temp[3];

     
    offset->x = temp[4] >> 16;
    offset->y = temp[5] >> 16;
  }