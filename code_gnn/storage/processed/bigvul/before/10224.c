  FT_Activate_Size( FT_Size  size )
  {
    FT_Face  face;


    if ( size == NULL )
      return FT_Err_Invalid_Argument;

    face = size->face;
    if ( face == NULL || face->driver == NULL )
      return FT_Err_Invalid_Argument;

     
     
    face->size = size;

    return FT_Err_Ok;
  }