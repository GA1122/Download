  sfnt_done_face( TT_Face  face )
  {
    FT_Memory     memory;
    SFNT_Service  sfnt;


    if ( !face )
      return;

    memory = face->root.memory;
    sfnt   = (SFNT_Service)face->sfnt;

    if ( sfnt )
    {
       
      if ( sfnt->free_psnames )
        sfnt->free_psnames( face );

       
      if ( sfnt->free_eblc )
        sfnt->free_eblc( face );
    }

#ifdef TT_CONFIG_OPTION_BDF
     
    tt_face_free_bdf_props( face );
#endif

     
    tt_face_done_kern( face );

     
    FT_FREE( face->ttc_header.offsets );
    face->ttc_header.count = 0;

     
    FT_FREE( face->dir_tables );
    face->num_tables = 0;

    {
      FT_Stream  stream = FT_FACE_STREAM( face );


       
      FT_FRAME_RELEASE( face->cmap_table );
      face->cmap_size = 0;
    }

    face->horz_metrics_size = 0;
    face->vert_metrics_size = 0;

     
    if ( face->vertical_info )
    {
      FT_FREE( face->vertical.long_metrics  );
      FT_FREE( face->vertical.short_metrics );
      face->vertical_info = 0;
    }

     
    FT_FREE( face->gasp.gaspRanges );
    face->gasp.numRanges = 0;

     
    if ( sfnt )
      sfnt->free_name( face );

     
    FT_FREE( face->root.family_name );
    FT_FREE( face->root.style_name );

     
    FT_FREE( face->root.available_sizes );
    FT_FREE( face->sbit_strike_map );
    face->root.num_fixed_sizes = 0;

    FT_FREE( face->postscript_name );

    face->sfnt = NULL;
  }
