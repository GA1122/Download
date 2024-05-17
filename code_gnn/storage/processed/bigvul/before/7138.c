  cf2_glyphpath_finalize( CF2_GlyphPath  glyphpath )
  {
    cf2_arrstack_finalize( &glyphpath->hintMoves );
  }
