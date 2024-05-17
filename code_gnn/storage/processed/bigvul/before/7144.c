  cf2_glyphpath_pushPrevElem( CF2_GlyphPath  glyphpath,
                              CF2_HintMap    hintmap,
                              FT_Vector*     nextP0,
                              FT_Vector      nextP1,
                              FT_Bool        close )
  {
    CF2_CallbackParamsRec  params;

    FT_Vector*  prevP0;
    FT_Vector*  prevP1;

    FT_Vector  intersection    = { 0, 0 };
    FT_Bool    useIntersection = FALSE;


    FT_ASSERT( glyphpath->prevElemOp == CF2_PathOpLineTo ||
               glyphpath->prevElemOp == CF2_PathOpCubeTo );

    if ( glyphpath->prevElemOp == CF2_PathOpLineTo )
    {
      prevP0 = &glyphpath->prevElemP0;
      prevP1 = &glyphpath->prevElemP1;
    }
    else
    {
      prevP0 = &glyphpath->prevElemP2;
      prevP1 = &glyphpath->prevElemP3;
    }

     
     
     
    if ( prevP1->x != nextP0->x || prevP1->y != nextP0->y )
    {
       
       
      useIntersection = cf2_glyphpath_computeIntersection( glyphpath,
                                                           prevP0,
                                                           prevP1,
                                                           nextP0,
                                                           &nextP1,
                                                           &intersection );
      if ( useIntersection )
      {
         
         
        *prevP1 = intersection;
      }
    }

    params.pt0 = glyphpath->currentDS;

    switch( glyphpath->prevElemOp )
    {
    case CF2_PathOpLineTo:
      params.op = CF2_PathOpLineTo;

       

      if ( close )
      {
         
        cf2_glyphpath_hintPoint( glyphpath,
                                 &glyphpath->firstHintMap,
                                 &params.pt1,
                                 glyphpath->prevElemP1.x,
                                 glyphpath->prevElemP1.y );
      }
      else
      {
        cf2_glyphpath_hintPoint( glyphpath,
                                 hintmap,
                                 &params.pt1,
                                 glyphpath->prevElemP1.x,
                                 glyphpath->prevElemP1.y );
      }

       
      if ( params.pt0.x != params.pt1.x || params.pt0.y != params.pt1.y )
      {
        glyphpath->callbacks->lineTo( glyphpath->callbacks, &params );

        glyphpath->currentDS = params.pt1;
      }
      break;

    case CF2_PathOpCubeTo:
      params.op = CF2_PathOpCubeTo;

       
      cf2_glyphpath_hintPoint( glyphpath,
                               hintmap,
                               &params.pt1,
                               glyphpath->prevElemP1.x,
                               glyphpath->prevElemP1.y );
      cf2_glyphpath_hintPoint( glyphpath,
                               hintmap,
                               &params.pt2,
                               glyphpath->prevElemP2.x,
                               glyphpath->prevElemP2.y );
      cf2_glyphpath_hintPoint( glyphpath,
                               hintmap,
                               &params.pt3,
                               glyphpath->prevElemP3.x,
                               glyphpath->prevElemP3.y );

      glyphpath->callbacks->cubeTo( glyphpath->callbacks, &params );

      glyphpath->currentDS = params.pt3;

      break;
    }

    if ( !useIntersection || close )
    {
       
       
       
       

      if ( close )
      {
         
         
        cf2_glyphpath_hintPoint( glyphpath,
                                 &glyphpath->firstHintMap,
                                 &params.pt1,
                                 nextP0->x,
                                 nextP0->y );
      }
      else
      {
        cf2_glyphpath_hintPoint( glyphpath,
                                 hintmap,
                                 &params.pt1,
                                 nextP0->x,
                                 nextP0->y );
      }

      if ( params.pt1.x != glyphpath->currentDS.x ||
           params.pt1.y != glyphpath->currentDS.y )
      {
         
        params.op  = CF2_PathOpLineTo;
        params.pt0 = glyphpath->currentDS;

         
        glyphpath->callbacks->lineTo( glyphpath->callbacks, &params );

        glyphpath->currentDS = params.pt1;
      }
    }

    if ( useIntersection )
    {
       
      *nextP0 = intersection;
    }
  }
