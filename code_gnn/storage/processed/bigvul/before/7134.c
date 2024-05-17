  cf2_glyphpath_closeOpenPath( CF2_GlyphPath  glyphpath )
  {
    if ( glyphpath->pathIsOpen )
    {
       
      glyphpath->pathIsClosing = TRUE;

      cf2_glyphpath_lineTo( glyphpath,
                            glyphpath->start.x,
                            glyphpath->start.y );

       
      if ( glyphpath->elemIsQueued )
        cf2_glyphpath_pushPrevElem( glyphpath,
                                    &glyphpath->hintMap,
                                    &glyphpath->offsetStart0,
                                    glyphpath->offsetStart1,
                                    TRUE );

       
      glyphpath->moveIsPending = TRUE;
      glyphpath->pathIsOpen    = FALSE;
      glyphpath->pathIsClosing = FALSE;
      glyphpath->elemIsQueued  = FALSE;
    }
  }
