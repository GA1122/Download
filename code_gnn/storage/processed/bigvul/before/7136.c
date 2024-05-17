  cf2_glyphpath_computeOffset( CF2_GlyphPath  glyphpath,
                               CF2_Fixed      x1,
                               CF2_Fixed      y1,
                               CF2_Fixed      x2,
                               CF2_Fixed      y2,
                               CF2_Fixed*     x,
                               CF2_Fixed*     y )
  {
    CF2_Fixed  dx = x2 - x1;
    CF2_Fixed  dy = y2 - y1;


     
     
    if ( glyphpath->font->reverseWinding )
    {
      dx = -dx;
      dy = -dy;
    }

    *x = *y = 0;

    if ( !glyphpath->darken )
        return;

     
    glyphpath->callbacks->windingMomentum +=
      cf2_getWindingMomentum( x1, y1, x2, y2 );

     
    if ( dx >= 0 )
    {
      if ( dy >= 0 )
      {
         

        if ( dx > 2 * dy )
        {
           
          *x = 0;
          *y = 0;
        }
        else if ( dy > 2 * dx )
        {
           
          *x = glyphpath->xOffset;
          *y = glyphpath->yOffset;
        }
        else
        {
           
          *x = FT_MulFix( cf2_floatToFixed( 0.7 ),
                          glyphpath->xOffset );
          *y = FT_MulFix( cf2_floatToFixed( 1.0 - 0.7 ),
                          glyphpath->yOffset );
        }
      }
      else
      {
         

        if ( dx > -2 * dy )
        {
           
          *x = 0;
          *y = 0;
        }
        else if ( -dy > 2 * dx )
        {
           
          *x = -glyphpath->xOffset;
          *y = glyphpath->yOffset;
        }
        else
        {
           
          *x = FT_MulFix( cf2_floatToFixed( -0.7 ),
                          glyphpath->xOffset );
          *y = FT_MulFix( cf2_floatToFixed( 1.0 - 0.7 ),
                          glyphpath->yOffset );
        }
      }
    }
    else
    {
      if ( dy >= 0 )
      {
         

        if ( -dx > 2 * dy )
        {
           
          *x = 0;
          *y = 2 * glyphpath->yOffset;
        }
        else if ( dy > -2 * dx )
        {
           
          *x = glyphpath->xOffset;
          *y = glyphpath->yOffset;
        }
        else
        {
           
          *x = FT_MulFix( cf2_floatToFixed( 0.7 ),
                          glyphpath->xOffset );
          *y = FT_MulFix( cf2_floatToFixed( 1.0 + 0.7 ),
                          glyphpath->yOffset );
        }
      }
      else
      {
         

        if ( -dx > -2 * dy )
        {
           
          *x = 0;
          *y = 2 * glyphpath->yOffset;
        }
        else if ( -dy > -2 * dx )
        {
           
          *x = -glyphpath->xOffset;
          *y = glyphpath->yOffset;
        }
        else
        {
           
          *x = FT_MulFix( cf2_floatToFixed( -0.7 ),
                          glyphpath->xOffset );
          *y = FT_MulFix( cf2_floatToFixed( 1.0 + 0.7 ),
                          glyphpath->yOffset );
        }
      }
    }
  }
