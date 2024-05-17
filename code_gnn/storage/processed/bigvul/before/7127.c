  T42_Size_Select( FT_Size   t42size,          
                   FT_ULong  strike_index )
  {
    T42_Size  size = (T42_Size)t42size;
    T42_Face  face = (T42_Face)t42size->face;
    FT_Error  error;


    FT_Activate_Size( size->ttsize );

    error = FT_Select_Size( face->ttf_face, (FT_Int)strike_index );
    if ( !error )
      t42size->metrics = face->ttf_face->size->metrics;

    return error;

  }
