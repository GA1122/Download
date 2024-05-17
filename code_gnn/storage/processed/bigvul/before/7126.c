  T42_Size_Init( FT_Size  size )          
  {
    T42_Size  t42size = (T42_Size)size;
    FT_Face   face    = size->face;
    T42_Face  t42face = (T42_Face)face;
    FT_Size   ttsize;
    FT_Error  error;


    error = FT_New_Size( t42face->ttf_face, &ttsize );
    t42size->ttsize = ttsize;

    FT_Activate_Size( ttsize );

    return error;
  }
