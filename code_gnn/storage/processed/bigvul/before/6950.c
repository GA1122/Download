  FT_New_Face_From_Resource( FT_Library    library,
                             const UInt8*  pathname,
                             FT_Long       face_index,
                             FT_Face*      aface )
  {
    OSType    file_type;
    FT_Error  error;


     
    file_type = get_file_type_from_path( pathname );
    if ( file_type == TTAG_LWFN )
      return FT_New_Face_From_LWFN( library, pathname, face_index, aface );

     
     
     

    error = FT_New_Face_From_Suitcase( library, pathname, face_index, aface );
    if ( error == 0 )
      return error;

     
     
    *aface = NULL;
    return 0;
  }
