  tt_face_done( FT_Face  ttface )            
  {
    TT_Face       face = (TT_Face)ttface;
    FT_Memory     memory;
    FT_Stream     stream;
    SFNT_Service  sfnt;


    if ( !face )
      return;

    memory = ttface->memory;
    stream = ttface->stream;
    sfnt   = (SFNT_Service)face->sfnt;

     
    if ( face->extra.finalizer )
      face->extra.finalizer( face->extra.data );

    if ( sfnt )
      sfnt->done_face( face );

     
    tt_face_done_loca( face );

    tt_face_free_hdmx( face );

     
    FT_FREE( face->cvt );
    face->cvt_size = 0;

     
    FT_FRAME_RELEASE( face->font_program );
    FT_FRAME_RELEASE( face->cvt_program );
    face->font_program_size = 0;
    face->cvt_program_size  = 0;

#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
    tt_done_blend( face );
    face->blend = NULL;
#endif
  }