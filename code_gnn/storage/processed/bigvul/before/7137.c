  cf2_glyphpath_curveTo( CF2_GlyphPath  glyphpath,
                         CF2_Fixed      x1,
                         CF2_Fixed      y1,
                         CF2_Fixed      x2,
                         CF2_Fixed      y2,
                         CF2_Fixed      x3,
                         CF2_Fixed      y3 )
  {
    CF2_Fixed  xOffset1, yOffset1, xOffset3, yOffset3;
    FT_Vector  P0, P1, P2, P3;


     
    cf2_glyphpath_computeOffset( glyphpath,
                                 glyphpath->currentCS.x,
                                 glyphpath->currentCS.y,
                                 x1,
                                 y1,
                                 &xOffset1,
                                 &yOffset1 );
    cf2_glyphpath_computeOffset( glyphpath,
                                 x2,
                                 y2,
                                 x3,
                                 y3,
                                 &xOffset3,
                                 &yOffset3 );

     
    glyphpath->callbacks->windingMomentum +=
      cf2_getWindingMomentum( x1, y1, x2, y2 );

     
    P0.x = glyphpath->currentCS.x + xOffset1;
    P0.y = glyphpath->currentCS.y + yOffset1;
    P1.x = x1 + xOffset1;
    P1.y = y1 + yOffset1;
     
    P2.x = x2 + xOffset3;
    P2.y = y2 + yOffset3;
    P3.x = x3 + xOffset3;
    P3.y = y3 + yOffset3;

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
    glyphpath->prevElemOp   = CF2_PathOpCubeTo;
    glyphpath->prevElemP0   = P0;
    glyphpath->prevElemP1   = P1;
    glyphpath->prevElemP2   = P2;
    glyphpath->prevElemP3   = P3;

     
    if ( cf2_hintmask_isNew( glyphpath->hintMask ) )
      cf2_hintmap_build( &glyphpath->hintMap,
                         glyphpath->hStemHintArray,
                         glyphpath->vStemHintArray,
                         glyphpath->hintMask,
                         glyphpath->hintOriginY,
                         FALSE );

    glyphpath->currentCS.x = x3;        
    glyphpath->currentCS.y = y3;
  }
