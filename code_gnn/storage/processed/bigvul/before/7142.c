  cf2_glyphpath_moveTo( CF2_GlyphPath  glyphpath,
                        CF2_Fixed      x,
                        CF2_Fixed      y )
  {
    cf2_glyphpath_closeOpenPath( glyphpath );

     
     
     
    glyphpath->currentCS.x = glyphpath->start.x = x;
    glyphpath->currentCS.y = glyphpath->start.y = y;

    glyphpath->moveIsPending = TRUE;

     
    if ( !cf2_hintmap_isValid( &glyphpath->hintMap ) ||
         cf2_hintmask_isNew( glyphpath->hintMask )   )
      cf2_hintmap_build( &glyphpath->hintMap,
                         glyphpath->hStemHintArray,
                         glyphpath->vStemHintArray,
                         glyphpath->hintMask,
                         glyphpath->hintOriginY,
                         FALSE );

     
    glyphpath->firstHintMap = glyphpath->hintMap;      
  }
