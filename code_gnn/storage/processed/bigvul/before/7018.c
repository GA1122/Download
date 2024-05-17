  Convert_Glyph( RAS_ARGS int  flipped )
  {
    int       i;
    unsigned  start;


    ras.fProfile = NULL;
    ras.joint    = FALSE;
    ras.fresh    = FALSE;

    ras.maxBuff  = ras.sizeBuff - AlignProfileSize;

    ras.numTurns = 0;

    ras.cProfile         = (PProfile)ras.top;
    ras.cProfile->offset = ras.top;
    ras.num_Profs        = 0;

    start = 0;

    for ( i = 0; i < ras.outline.n_contours; i++ )
    {
      PProfile  lastProfile;
      Bool      o;


      ras.state    = Unknown_State;
      ras.gProfile = NULL;

      if ( Decompose_Curve( RAS_VARS (unsigned short)start,
                                     ras.outline.contours[i],
                                     flipped ) )
        return FAILURE;

      start = ras.outline.contours[i] + 1;

       
      if ( FRAC( ras.lastY ) == 0 &&
           ras.lastY >= ras.minY  &&
           ras.lastY <= ras.maxY  )
        if ( ras.gProfile                        &&
             ( ras.gProfile->flags & Flow_Up ) ==
               ( ras.cProfile->flags & Flow_Up ) )
          ras.top--;
         
         

      lastProfile = ras.cProfile;
      if ( ras.cProfile->flags & Flow_Up )
        o = IS_TOP_OVERSHOOT( ras.lastY );
      else
        o = IS_BOTTOM_OVERSHOOT( ras.lastY );
      if ( End_Profile( RAS_VARS o ) )
        return FAILURE;

       
      if ( ras.gProfile )
        lastProfile->next = ras.gProfile;
    }

    if ( Finalize_Profile_Table( RAS_VAR ) )
      return FAILURE;

    return (Bool)( ras.top < ras.maxBuff ? SUCCESS : FAILURE );
  }
