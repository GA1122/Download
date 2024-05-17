  cf2_builder_cubeTo( CF2_OutlineCallbacks      callbacks,
                      const CF2_CallbackParams  params )
  {
     
    CF2_Outline   outline = (CF2_Outline)callbacks;
    CFF_Builder*  builder;


    FT_ASSERT( outline && outline->decoder );
    FT_ASSERT( params->op == CF2_PathOpCubeTo );

    builder = &outline->decoder->builder;

    if ( !builder->path_begun )
    {
       
       
      cff_builder_start_point( builder,
                               params->pt0.x,
                               params->pt0.y );
    }

     
    cff_check_points( builder, 3 );

    cff_builder_add_point( builder,
                           params->pt1.x,
                           params->pt1.y, 0 );
    cff_builder_add_point( builder,
                           params->pt2.x,
                           params->pt2.y, 0 );
    cff_builder_add_point( builder,
                           params->pt3.x,
                           params->pt3.y, 1 );
  }
