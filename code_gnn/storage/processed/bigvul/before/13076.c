  cf2_builder_moveTo( CF2_OutlineCallbacks      callbacks,
                      const CF2_CallbackParams  params )
  {
     
    CF2_Outline   outline = (CF2_Outline)callbacks;
    CFF_Builder*  builder;

    (void)params;         


    FT_ASSERT( outline && outline->decoder );
    FT_ASSERT( params->op == CF2_PathOpMoveTo );

    builder = &outline->decoder->builder;

     
    cff_builder_close_contour( builder );
    builder->path_begun = 0;
  }
