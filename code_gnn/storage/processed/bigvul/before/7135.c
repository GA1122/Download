  cf2_glyphpath_computeIntersection( CF2_GlyphPath     glyphpath,
                                     const FT_Vector*  u1,
                                     const FT_Vector*  u2,
                                     const FT_Vector*  v1,
                                     const FT_Vector*  v2,
                                     FT_Vector*        intersection )
  {
     

#define cf2_perp( a, b )                                    \
          ( FT_MulFix( a.x, b.y ) - FT_MulFix( a.y, b.x ) )

   
#define CF2_CS_SCALE( x )         \
          ( ( (x) + 0x10 ) >> 5 )

    FT_Vector  u, v, w;       
    CF2_Fixed  denominator, s;


    u.x = CF2_CS_SCALE( u2->x - u1->x );
    u.y = CF2_CS_SCALE( u2->y - u1->y );
    v.x = CF2_CS_SCALE( v2->x - v1->x );
    v.y = CF2_CS_SCALE( v2->y - v1->y );
    w.x = CF2_CS_SCALE( v1->x - u1->x );
    w.y = CF2_CS_SCALE( v1->y - u1->y );

    denominator = cf2_perp( u, v );

    if ( denominator == 0 )
      return FALSE;            

    s = FT_DivFix( cf2_perp( w, v ), denominator );

    intersection->x = u1->x + FT_MulFix( s, u2->x - u1->x );
    intersection->y = u1->y + FT_MulFix( s, u2->y - u1->y );

     

    if ( u1->x == u2->x                                                     &&
         cf2_fixedAbs( intersection->x - u1->x ) < glyphpath->snapThreshold )
      intersection->x = u1->x;
    if ( u1->y == u2->y                                                     &&
         cf2_fixedAbs( intersection->y - u1->y ) < glyphpath->snapThreshold )
      intersection->y = u1->y;

    if ( v1->x == v2->x                                                     &&
         cf2_fixedAbs( intersection->x - v1->x ) < glyphpath->snapThreshold )
      intersection->x = v1->x;
    if ( v1->y == v2->y                                                     &&
         cf2_fixedAbs( intersection->y - v1->y ) < glyphpath->snapThreshold )
      intersection->y = v1->y;

     
    if ( cf2_fixedAbs( intersection->x - ( u2->x + v1->x ) / 2 ) >
           glyphpath->miterLimit                                   ||
         cf2_fixedAbs( intersection->y - ( u2->y + v1->y ) / 2 ) >
           glyphpath->miterLimit                                   )
      return FALSE;

    return TRUE;
  }
