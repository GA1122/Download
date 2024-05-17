  cf2_glyphpath_lineTo( CF2_GlyphPath  glyphpath,
                        CF2_Fixed      x,
                        CF2_Fixed      y )
  {
    CF2_Fixed  xOffset, yOffset;
    FT_Vector  P0, P1;
    FT_Bool    newHintMap;

     

     
    newHintMap = cf2_hintmask_isNew( glyphpath->hintMask ) &&
                 !glyphpath->pathIsClosing;

     

     

    if ( glyphpath->currentCS.x == x &&
         glyphpath->currentCS.y == y &&
         !newHintMap                 )
       
      return;

    cf2_glyphpath_computeOffset( glyphpath,
                                 glyphpath->currentCS.x,
                                 glyphpath->currentCS.y,
                                 x,
                                 y,
                                 &xOffset,
                                 &yOffset );

     
    P0.x = glyphpath->currentCS.x + xOffset;
    P0.y = glyphpath->currentCS.y + yOffset;
    P1.x = x + xOffset;
    P1.y = y + yOffset;

    if ( glyphpath->moveIsPending )
    {
       
      cf2_glyphpath_pushMove( glyphpath, P0 );

      glyphpath->moveIsPending = FALSE;   
      glyphpath->pathIsOpen    = TRUE;

      glyphpath->offsetStart1 = P1;               
    }

    if ( glyphpath->elemIsQueued )
    {
      FT_ASSERT( cf2_hintmap_isValid( &glyphpath->hintMap ) );

      cf2_glyphpath_pushPrevElem( glyphpath,
                                  &glyphpath->hintMap,
                                  &P0,
                                  P1,
                                  FALSE );
    }

     
    glyphpath->elemIsQueued = TRUE;
    glyphpath->prevElemOp   = CF2_PathOpLineTo;
    glyphpath->prevElemP0   = P0;
    glyphpath->prevElemP1   = P1;

     
    if ( newHintMap )
      cf2_hintmap_build( &glyphpath->hintMap,
                         glyphpath->hStemHintArray,
                         glyphpath->vStemHintArray,
                         glyphpath->hintMask,
                         glyphpath->hintOriginY,
                         FALSE );

    glyphpath->currentCS.x = x;      
    glyphpath->currentCS.y = y;
  }
