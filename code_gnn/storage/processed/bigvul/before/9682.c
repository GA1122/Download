  destroy_face( FT_Memory  memory,
                FT_Face    face,
                FT_Driver  driver )
  {
    FT_Driver_Class  clazz = driver->clazz;


     
    if ( face->autohint.finalizer )
      face->autohint.finalizer( face->autohint.data );

     
     
    while ( face->glyph )
      FT_Done_GlyphSlot( face->glyph );

     
    FT_List_Finalize( &face->sizes_list,
                      (FT_List_Destructor)destroy_size,
                      memory,
                      driver );
    face->size = 0;

     
    if ( face->generic.finalizer )
      face->generic.finalizer( face );

     
    destroy_charmaps( face, memory );

     
    if ( clazz->done_face )
      clazz->done_face( face );

     
    FT_Stream_Free(
      face->stream,
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM ) != 0 );

    face->stream = 0;

     
    if ( face->internal )
    {
      FT_FREE( face->internal );
    }
    FT_FREE( face );
  }
