  cf2_builder_lineTo( CF2_OutlineCallbacks      callbacks,
                      const CF2_CallbackParams  params )
  {
     
    CF2_Outline   outline = (CF2_Outline)callbacks;
    CFF_Builder*  builder;


    FT_ASSERT( outline && outline->decoder );
    FT_ASSERT( params->op == CF2_PathOpLineTo );

    builder = &outline->decoder->builder;

    if ( !builder->path_begun )
    {
       
       
      cff_builder_start_point( builder,
                               params->pt0.x,
                               params->pt0.y );
    }

     
    cff_builder_add_point1( builder,
                            params->pt1.x,
                            params->pt1.y );
  }