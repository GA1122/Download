  cf2_glyphpath_pushMove( CF2_GlyphPath  glyphpath,
                          FT_Vector      start )
  {
    CF2_CallbackParamsRec  params;


    params.op  = CF2_PathOpMoveTo;
    params.pt0 = glyphpath->currentDS;

     
     
    if ( !cf2_hintmap_isValid( &glyphpath->hintMap ) )
    {
       
       
      cf2_glyphpath_moveTo( glyphpath,
                            glyphpath->start.x,
                            glyphpath->start.y );
    }

    cf2_glyphpath_hintPoint( glyphpath,
                             &glyphpath->hintMap,
                             &params.pt1,
                             start.x,
                             start.y );

     
    glyphpath->callbacks->moveTo( glyphpath->callbacks, &params );

    glyphpath->currentDS    = params.pt1;
    glyphpath->offsetStart0 = start;
  }
